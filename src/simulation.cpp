#include "simulation.h"

//No acceleration for now, which makes interpolation really easy. 😎
void simulation::interpolate(const float& time, glm::vec2& p, const glm::vec2& v){
    p.x = v.x*time + p.x;
    p.y = v.y*time + p.y;
}

change::change(event& e, int index, const float& time){
    this->ball = e.balls[index].ball;
    this->position = e.balls[index].position;
    this->velocity = e.balls[index].velocity;
    float Δt = time - *e.time;
    simulation::interpolate(Δt, this->position, this->velocity);
}

moment_i::moment_i(int& t){
    this->t = t;
}

void moment_f::add_event(collision_type type, int ball_a, int ball_b){
    events.emplace(events.end(), type, ball_a, ball_b, &t);
}

bool simulation::predict(){
    collision.t = timer;
    collision_type type = NONE;

    for(int i = 0; i < objects.size(); i++){
        objects[i].move(sim_time - objects[i].time);
    }

    std::set<int> exceptions = {};
    for(int i = 0; i < objects.size(); i++){
        exceptions.emplace(i);
        
        bool new_time = false;
        if(objects[i].next_collision(collision.t, type, new_time)){
            if(new_time){
                collision.events.clear();
            }
            collision.add_event(type, i, -1);
        }

        for(int x = 0; x < objects.size(); x++){
            if(exceptions.find(x) != exceptions.end()) continue;
            float Δx, Δy, Δvx, Δvy, a, b, c, Δx2, Δy2;
            Δx = objects[i].position.x - objects[x].position.x;
            Δy = objects[i].position.y - objects[x].position.y;
            Δvx = objects[i].velocity.x - objects[x].velocity.x;
            Δvy = objects[i].velocity.y - objects[x].velocity.y;
            b = 2*(Δx*Δvx + Δy*Δvy);
            if(b >= 0) continue;
            //Computing the zeroes for every single ball is a lot. We fix this by taking the derivative of the distance function at t = 0 (AKA b).
            //If its >= 0, (the balls are increasing in distance at this instant) then we continue.
            Δx2 = pow(Δx, 2);
            Δy2 = pow(Δy, 2);
            a = pow(Δvx, 2) + pow(Δvy, 2);
            c = Δx2 + Δy2 - pow(objects[i].radius + objects[x].radius, 2);
            float discriminant = pow(b, 2) - 4*a*c;
            if(discriminant <= 0) continue;
            //We always take the smaller one, even if it is negative, cause the larger one will always require the balls to phase thru each other.
            float new_T = std::min((-b + sqrt(discriminant))/(2*a), (-b - sqrt(discriminant))/(2*a)) + sim_time;
            if(new_T <= collision.t && new_T >= sim_time){
                if(abs(collision.t - new_T) > 1e-7f) {
                    collision.events.clear();
                    collision.t = new_T;
                }
                type = BALL;
                collision.add_event(type, i, x);
            }
        }
    }
    if(type == NONE) return false;
    return true;
}

simulation::simulation(std::vector<float>& points, std::string source, float aspect, int timer){
    this->time = 0;
    this->timer = timer;
    this->aspect = aspect;
    for(int i = 0; i < points.size(); i+=12){
        objects_m.emplace_back(points[i], points[i + 1], points[i + 2], points[i + 3], points[i + 4], points[i + 5], points[i + 6], points[i + 7], source, aspect, points[i + 8], points[i + 9], points[i + 10], points[i + 11]);
    }
}

void simulation::simulate(){
    sim_time = 0.0f;

    for(int i = 0; i < objects_m.size(); i++){
        objects.emplace_back(objects_m[i].ball);
    }

    while(predict()){
        sim_time = collision.t;
        std::cout << (sim_time*100.0f)/(float)timer << "%\n";
        int framed_time = (int)std::round(collision.t);
        auto it = moments.emplace(framed_time);
        for(auto i = collision.events.begin(); i != collision.events.end(); i++){
            resolve(*i);
            for(int x = 0; x < 2; x++){
                if(i->balls[x].ball == -1) continue;
                auto check = it.first->events.find(i->balls[x].ball);
                if(check != it.first->events.end()) it.first->events.erase(check);
                it.first->events.emplace(*i, x, (float)framed_time);
            }
        }
    }
}

void simulation::resolve(event& e){
    if(e.type == NONE){
        return;
    } else {
        objects[e.balls[0].ball].move(*e.time - objects[e.balls[0].ball].time);
        e.balls[0].position = objects[e.balls[0].ball].position;
        if(e.type == WALL){
            objects[e.balls[0].ball].velocity.x = objects[e.balls[0].ball].velocity.x * -1.0f;
            e.balls[0].velocity = objects[e.balls[0].ball].velocity;
        } else if(e.type == CEILING){
            objects[e.balls[0].ball].velocity.y = objects[e.balls[0].ball].velocity.y * -1.0f;
            e.balls[0].velocity = objects[e.balls[0].ball].velocity;
        }else if(e.type == BALL){
            objects[e.balls[1].ball].move(*e.time - objects[e.balls[1].ball].time);
            e.balls[1].position = objects[e.balls[1].ball].position;

            glm::vec2 dir;
            float v1n, v2n, v1nf, v2nf, masses;

            dir = {objects[e.balls[0].ball].position - objects[e.balls[1].ball].position};
            dir = glm::normalize(dir);

            v1n = glm::dot(dir, objects[e.balls[0].ball].velocity);
            v2n = glm::dot(dir, objects[e.balls[1].ball].velocity);

            objects[e.balls[0].ball].velocity = objects[e.balls[0].ball].velocity - (v1n * dir);
            objects[e.balls[1].ball].velocity = objects[e.balls[1].ball].velocity - (v2n * dir);

            masses = objects[e.balls[0].ball].mass + objects[e.balls[1].ball].mass;

            v1nf = (v1n * (objects[e.balls[0].ball].mass - objects[e.balls[1].ball].mass))/masses + (2 * objects[e.balls[1].ball].mass * v2n)/masses;
            v2nf = (v2n * (objects[e.balls[1].ball].mass - objects[e.balls[0].ball].mass))/masses + (2 * objects[e.balls[0].ball].mass * v1n)/masses;

            objects[e.balls[0].ball].velocity = (v1nf*dir) + objects[e.balls[0].ball].velocity;
            objects[e.balls[1].ball].velocity = (v2nf*dir) + objects[e.balls[1].ball].velocity;

            e.balls[0].velocity = objects[e.balls[0].ball].velocity;
            e.balls[1].velocity = objects[e.balls[1].ball].velocity;
        }
    }
}

bool simulation::run(){
    CALL(glClear(GL_COLOR_BUFFER_BIT));

    std::set<int> exclusions = {};
    auto i = moments.find(time);
    if(i != moments.end()){
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