#include "simulation.h"

change::change(event& e, int index){
    this->ball = e.balls[index].ball;
    this->position = e.balls[index].position;
    this->velocity = e.balls[index].velocity;
    float Δt = std::ceil(*e.time) - *e.time;
    this->position.interpolate(Δt, this->velocity);
}

moment_i::moment_i(int& t){
    this->t = t;
}

moment_f::moment_f(float& t){
    this->t = t;
}

std::list<event>::iterator moment_f::add_event(event& e) const{
    std::list<event>::iterator returner = events.insert(events.begin(), e);

    returner->time = const_cast<float*>(&this->t);
    return returner;
}

simulation::simulation(std::vector<float>& points, std::string source, float aspect, int timer){
    this->time = 0;
    this->timer = timer;
    this->aspect = aspect;
    for(int i = 0; i < points.size(); i+=12){
        objects_m.emplace_back(points[i], points[i + 1], points[i + 2], points[i + 3], points[i + 4], points[i + 5], points[i + 6], points[i + 7], source, aspect, points[i + 8], points[i + 9], points[i + 10], points[i + 11]);
    }
}

void simulation::predict(const int& ball, const std::set<int>& exceptions){
    float t = timer;
    event collision = objects[ball].next_collision(t, ball);
    for(int i = 0; i < objects.size(); i++){
        if(exceptions.find(i) != exceptions.end()) continue;
        //Make sure both balls are at the same time. We also need to make sure that when we move the ball to the right time, we dont move past the time of its next collision
        //When in doubt this is a good place to check out for bugs
        if(objects[ball].time != objects[i].time) objects[i].move(objects[ball].time - objects[i].time);
        float Δx, Δy, Δvx, Δvy, a, b, c, Δx2, Δy2;
        Δx = objects[ball].position.x - objects[i].position.x;
        Δy = objects[ball].position.y - objects[i].position.y;
        Δvx = objects[ball].velocity.x - objects[i].velocity.x;
        Δvy = objects[ball].velocity.y - objects[i].velocity.y;
        Δx2 = pow(Δx, 2);
        Δy2 = pow(Δy, 2);
        a = pow(Δvx, 2) + pow(Δvy, 2);
        b = 2*(Δx*Δvx + Δy*Δvy);
        c = Δx2 + Δy2 - pow(objects[ball].radius + objects[i].radius, 2);
        //Computing the disrcriminant for every single ball is a lot. Coming up with an optimization in the future will make this process more worth it
        float discriminant = pow(b, 2) - 4*a*c;
        if(discriminant <= 0) continue;
        //We always take the smaller one, even if it is negative, cause the larger one will always require the balls to phase thru each other.
        float new_T = std::min((-b + sqrt(discriminant))/(2*a), (-b - sqrt(discriminant))/(2*a)) + objects[ball].time;
        float bT = (!objects[i].collision) ? timer : *objects[i].collision.value()->time;
        if(new_T < t && new_T < bT && new_T > objects[ball].time) {
            t = new_T;
            collision.balls[1].ball = i;
            collision.type = BALL;
        }
    }
    if(collision.type != NONE){
        if(key_moments.find(t) == key_moments.end()) key_moments.emplace(t);
        objects[ball].collision = key_moments.find(t)->add_event(collision);
    } else {
        objects[ball].collision.reset();
    }

    if(collision.type == BALL){
        if(objects[collision.balls[1].ball].collision && objects[collision.balls[1].ball].collision.value()->type == BALL){
            bool index = (collision.balls[1].ball == objects[collision.balls[1].ball].collision.value()->balls[1].ball) ? false : true;
            objects[objects[collision.balls[1].ball].collision.value()->balls[index].ball].collision.reset();
            queue.emplace(objects[collision.balls[1].ball].collision.value()->balls[index].ball, std::set<int>{objects[collision.balls[1].ball].collision.value()->balls[index].ball, collision.balls[0].ball, collision.balls[1].ball});
        }
        if(objects[collision.balls[1].ball].collision) objects[collision.balls[1].ball].collision.value()->type = NONE;
        objects[collision.balls[1].ball].collision = objects[ball].collision;
    }
}

void simulation::simulate(){
    for(int i = 0; i < objects_m.size(); i++){
        objects.emplace_back(objects_m[i].ball);
    }
    std::set<int> exceptions = {};
    for(int i = 0; i < objects.size(); i++){
        exceptions.emplace(i);
        queue.emplace(i, exceptions);
    }
    while(!queue.empty()){
        std::pair<int, std::set<int>> x = queue.top();
        queue.pop();
        predict(x.first, x.second);
    }
    
    bool quit = false;
    while(!quit){
        quit = false;
        for(int i = 0; i < objects.size(); i++){
            if(objects[i].collision) quit = resolve(*objects[i].collision.value());
            while(!queue.empty()){
                std::pair<int, std::set<int>> x = queue.top();
                queue.pop();
                predict(x.first, x.second);
            }
        }
    }

    //Please help me
    for(auto i = key_moments.begin(); i != key_moments.end(); i++){
        int tick = (int)std::ceil(i->t);
        auto it = framed_moments.find(tick);
        if(it == framed_moments.end()) { it = framed_moments.emplace(tick).first; }
        for(auto x = i->events.begin(); x != i->events.end(); x++){
            if(x->type == NONE) continue;
            int max = (x->type == BALL) ? 2 : 1;
            for(int j = 0; j < max; j++){
                auto poop = it->events.find(x->balls[j].ball);
                if(poop != it->events.end()){
                    //sus
                    it->events.erase(poop);
                }
                it->events.emplace(*x, j);
            }
        }
    }
}

bool simulation::resolve(event& e){
    if(e.type == NONE){
        return false;
    } else {
        objects[e.balls[0].ball].move(*e.time - objects[e.balls[0].ball].time);
        e.balls[0].position = objects[e.balls[0].ball].position;

        if(e.type == WALL){
            objects[e.balls[0].ball].velocity.x = objects[e.balls[0].ball].velocity.x * -1.0f;
            e.balls[0].velocity = objects[e.balls[0].ball].velocity;


            queue.emplace(e.balls[0].ball, std::set<int>{e.balls[0].ball});
        } else if(e.type == CEILING){
            objects[e.balls[0].ball].velocity.y = objects[e.balls[0].ball].velocity.y * -1.0f;
            e.balls[0].velocity = objects[e.balls[0].ball].velocity;

            queue.emplace(e.balls[0].ball, std::set<int>{e.balls[0].ball});
        }else if(e.type == BALL){
            objects[e.balls[1].ball].move(*e.time - objects[e.balls[1].ball].time);
            e.balls[1].position = objects[e.balls[1].ball].position;

            float x1, x2, y1, y2, θ;
            x1 = objects[e.balls[0].ball].position.x;
            x2 = objects[e.balls[1].ball].position.x;
            y1 = objects[e.balls[0].ball].position.y;
            y2 = objects[e.balls[1].ball].position.y;
            bool x, y, flat;
            x = x2 > x1;
            y = y2 > y1;
            flat = x2 == x1;
            if(!flat){
                θ = atan((y2 - y1)/(x2 - x1));
                if(y){
                    if(x){
                        θ = -θ;
                    } else {
                        θ = abs(θ);
                    }
                } else {
                    if(x){
                        θ = abs(θ);
                    } else {
                        θ = -θ;
                    }
                }
            } else {
                θ = M_PI/2.0f;
            }
            float v1x, v2x, v1y, v2y, v1n, v2n, v1t, v2t, v1nf, v2nf, sinθ, cosθ, m1, m2, masses;
            v1x = objects[e.balls[0].ball].velocity.x;
            v1y = objects[e.balls[0].ball].velocity.y;
            v2x = objects[e.balls[1].ball].velocity.x;
            v2y = objects[e.balls[1].ball].velocity.y;
            sinθ = sin(θ);
            cosθ = cos(θ);
            v1n = (v1x * cosθ) - (v1y * sinθ);
            v1t = (v1x * sinθ) + (v1y * cosθ);
            v2n = (v2x * cosθ) - (v2y * sinθ);
            v2t = (v2x * sinθ) + (v2y * cosθ);
            m1 = objects[e.balls[0].ball].mass;
            m2 = objects[e.balls[1].ball].mass;
            masses = m1 + m2;
            v1nf = (v1n * (m1 - m2))/masses + (2 * m2 * v2n)/masses;
            v2nf = (v2n * (m2 - m1))/masses + (2 * m1 * v1n)/masses;
            sinθ = sin(-θ);
            cosθ = cos(-θ);

            objects[e.balls[0].ball].velocity.x = (v1nf * cosθ) - (v1t * sinθ);
            objects[e.balls[0].ball].velocity.y = (v1nf * sinθ) + (v1t * cosθ);
            objects[e.balls[1].ball].velocity.x = (v2nf * cosθ) - (v2t * sinθ);
            objects[e.balls[1].ball].velocity.y = (v2nf * sinθ) + (v2t * cosθ);

            e.balls[0].velocity = objects[e.balls[0].ball].velocity;
            e.balls[1].velocity = objects[e.balls[1].ball].velocity;

            queue.emplace(e.balls[0].ball, std::set<int>{e.balls[0].ball, e.balls[1].ball});
            queue.emplace(e.balls[1].ball, std::set<int>{e.balls[0].ball, e.balls[1].ball});
        }
    }
    return true;
}

bool simulation::run(){
    CALL(glClear(GL_COLOR_BUFFER_BIT));

    std::set<int> exclusions = {};
    auto i = framed_moments.find(time);
    if(i != framed_moments.end()){
        for(auto x = i->events.begin(); x != i->events.end(); x++){
            exclusions.insert(x->ball);
            objects_m[x->ball].ball.position = x->position;
            objects_m[x->ball].ball.velocity = x->velocity;
            objects_m[x->ball].update();
        }
    }

    for(int i = 0; i < objects_m.size(); i++){
        if(exclusions.find(i) != exclusions.end()) continue;
        objects_m[i].tick();
    }

    time++;
    if(time > timer) return false;
    return true;
}