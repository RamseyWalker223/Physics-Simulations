#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//No acceleration for now, which makes interpolation really easy. 😎
void t_vector::interpolate(const float& time, const t_vector& v){
    this->x = v.x*time + this->x;
    this->y = v.y*time + this->y;
}

void particle::interpolate(float& time){
    position.interpolate(time - t, velocities[0]);
    t = time;
}

event::event(float t, std::vector<int>& particles, const std::vector<t_vector>& positions, const std::vector<t_vector>& velocities, int axis){
    this->t = t;
    this->particles = particles;
    this->positions = positions;
    this->velocities = velocities;
    this->axis = axis;
}

event::event(const std::vector<particle>& objects){
    this->t = 0;
    this->axis = -1;
    for(int i = 0; i < objects.size(); i++){
        particles.emplace_back(i);
        positions.emplace_back(objects[i].position);
        velocities.emplace_back(objects[i].velocities[0]);
    }
}

event::event(const std::vector<event>& events){
    float time = std::ceil(events[0].t);
    this->t = time;
    this->axis = events[0].axis;
    for(int i = 0; i < events.size(); i++){
        for(int j = 0; j < events[i].particles.size(); j++){
            particles.emplace_back(events[i].particles[j]);
            positions.emplace_back(events[i].positions[j]);
            velocities.emplace_back(events[i].velocities[j]);
            positions[positions.size() - 1].interpolate(time - events[0].t, velocities[velocities.size() - 1]);
        }
    }
}

event::event(std::map<int, event>& events){
    float time = events[0].t;
    this->t = time;
    this->axis = events[0].axis;
    add(events);
}

//This can be optimized
void event::add(std::map<int, event>& events){
    for(int i = 0; i < events.size(); i++){
        for(int j = 0; j < events[i].particles.size(); j++){
            particles.emplace_back(events[i].particles[j]);
            positions.emplace_back(events[i].positions[j]);
            velocities.emplace_back(events[i].velocities[j]);
            //Remove this line if you want to be a sneaky little boy😈
            positions[positions.size() - 1].interpolate((int)std::ceil(this->t) - events[0].t, velocities[velocities.size() - 1]);
        }
    }
    this->t = events[0].t;
}

void simulation::collide(event& e){
    //For now only two particles can be involved in a collision, in the future we'll handle cases where more than one can be involved.
    //We'll use a tree structure for that
    objects[e.particles[0]].interpolate(e.t);
    objects[e.particles[1]].interpolate(e.t);
    float x1, x2, y1, y2, θ;
    x1 = objects[e.particles[0]].position.x;
    x2 = objects[e.particles[1]].position.x;
    y1 = objects[e.particles[0]].position.y;
    y2 = objects[e.particles[1]].position.y;
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
    v1x = objects[e.particles[0]].velocities[0].x;
    v1y = objects[e.particles[0]].velocities[0].y;
    v2x = objects[e.particles[1]].velocities[0].x;
    v2y = objects[e.particles[1]].velocities[0].y;
    sinθ = sin(θ);
    cosθ = cos(θ);
    v1n = (v1x * cosθ) - (v1y * sinθ);
    v1t = (v1x * sinθ) + (v1y * cosθ);
    v2n = (v2x * cosθ) - (v2y * sinθ);
    v2t = (v2x * sinθ) + (v2y * cosθ);
    m1 = objects[e.particles[0]].mass;
    m2 = objects[e.particles[1]].mass;
    masses = m1 + m2;
    v1nf = (v1n * (m1 - m2))/masses + (2 * m2 * v2n)/masses;
    v2nf = (v2n * (m2 - m1))/masses + (2 * m1 * v1n)/masses;
    sinθ = sin(-θ);
    cosθ = cos(-θ);
    objects[e.particles[0]].velocities[0].x = (v1nf * cosθ) - (v1t * sinθ);
    objects[e.particles[0]].velocities[0].y = (v1nf * sinθ) + (v1t * cosθ);
    objects[e.particles[1]].velocities[0].x = (v2nf * cosθ) - (v2t * sinθ);
    objects[e.particles[1]].velocities[0].y = (v2nf * sinθ) + (v2t * cosθ);

    objects[e.particles[0]].collision = nullptr;
    objects[e.particles[1]].collision = nullptr;
    objects[e.particles[0]].collision_index = -1;
    objects[e.particles[1]].collision_index = -1;

    e.positions.emplace_back(objects[e.particles[0]].position);
    e.positions.emplace_back(objects[e.particles[1]].position);
    e.velocities.emplace_back(objects[e.particles[0]].velocities[0]);
    e.velocities.emplace_back(objects[e.particles[1]].velocities[0]);

    predict(e.particles[0], {e.particles[0], e.particles[1]});
    predict(e.particles[1], {e.particles[0], e.particles[1]});
}

void simulation::collide_wall(event& e){
    objects[e.particles[0]].interpolate(e.t);
    if(e.axis == 1){
        objects[e.particles[0]].velocities[0].y*=-1;
    } else {
        objects[e.particles[0]].velocities[0].x*=-1;
    }
    e.positions.emplace_back(objects[e.particles[0]].position);
    e.velocities.emplace_back(objects[e.particles[0]].velocities[0]);
    predict(e.particles[0], {e.particles[0]});
}

simulation::simulation(std::vector<float>& points, std::string source, float aspect, int time){
    this->time = time;
    this->aspect = aspect;
    timer = 0;
    for(int i = 0; i < points.size(); i+=12){
        objects.emplace_back(points[i], points[i + 1], points[i + 2], points[i + 3], points[i + 4], points[i + 5], points[i + 6], points[i + 7], source, aspect, points[i + 8], points[i + 9], points[i + 10], points[i + 11]);
    }
    key_moments[0].emplace(0, objects);

    simulate();
}

simulation::simulation(int ball_X, int ball_Y, float mass_range, std::string source, float aspect, int time){
    this->time = time;
    this->timer = 0;
    this->aspect = aspect;
    float x, y, Δx, Δy, max_radius;
    Δx = (2*aspect/(ball_X + 1));
    Δy = (2.00f/(ball_Y + 1));
    x = Δx;
    y = Δy;
    max_radius = std::min(x, y)/2.3f;
    while(y < 2.00f){
        while(x < 2*aspect){
            objects.emplace_back(x-aspect, y-1.00f, 0.01f, 0.017f, 0, 0, mass_range, max_radius, source, aspect, 1, 0, 0, 1);
            x+=Δx;
        }
        x = Δx;
        y+=Δy;
    }
    key_moments[0].emplace(0, objects);
    simulate();
}

void simulation::simulate(){
    std::unordered_set<int> exclusions = {};
    for(int i = 0; i < objects.size(); i++){
        exclusions.insert(i);
        predict(i, exclusions);
    }
    auto it = key_moments.begin();
    it++;
    while(it != key_moments.end() && it->first < time){
        auto i = it->second.begin();
        while(i != it->second.end()){
            if(i->second.axis == 2){
                collide(i->second);
            } else {
                collide_wall(i->second);
            }
            i++; 
        }
        it++;
    }
    //23.1331902
    //Convert these key moments into interpolated frames
    auto i = key_moments.begin();
    while(i != key_moments.end()){
        int key = (int)std::ceil(i->first);
        if(framed_moments.find(key) == framed_moments.end()){
            framed_moments.emplace(key, i->second);
        } else {
            framed_moments[key].add(i->second);
        }
        i++;
    }
}

void simulation::predict(int ball, const std::unordered_set<int>& exceptions){
    float T = (objects[ball].collision == nullptr) ? time : (objects[ball].collision->t == objects[ball].t) ? time : objects[ball].collision->t;
    int type = -1;
    float dx = (objects[ball].velocities[0].x > 0) ? aspect - objects[ball].radius - objects[ball].position.x : -aspect + objects[ball].radius - objects[ball].position.x;
    float new_T = dx/objects[ball].velocities[0].x + objects[ball].t;
    if(new_T < T){
        T = new_T;
        type = 0;
    }
    float dy = (objects[ball].velocities[0].y > 0) ? 1 - objects[ball].radius - objects[ball].position.y : -1 + objects[ball].radius - objects[ball].position.y;
    new_T = dy/objects[ball].velocities[0].y + objects[ball].t;
    if(new_T < T){
        T = new_T;
        type = 1;
    }
    std::vector<int> balls = {ball};
    for(int i = 0; i < objects.size(); i++){
        if(exceptions.find(i) != exceptions.end()) continue;
        //Make sure both balls are at the same time.
        if(objects[ball].t != objects[i].t) objects[i].interpolate(objects[ball].t);
        float Δx, Δy, Δvx, Δvy, a, b, c, Δx2, Δy2;
        Δx = objects[ball].position.x - objects[i].position.x;
        Δy = objects[ball].position.y - objects[i].position.y;
        Δvx = objects[ball].velocities[0].x - objects[i].velocities[0].x;
        Δvy = objects[ball].velocities[0].y - objects[i].velocities[0].y;
        Δx2 = pow(Δx, 2);
        Δy2 = pow(Δy, 2);
        a = pow(Δvx, 2) + pow(Δvy, 2);
        b = 2*(Δx*Δvx + Δy*Δvy);
        c = Δx2 + Δy2 - pow(objects[ball].radius + objects[i].radius, 2);
        //Computing the disrcriminant for every single ball is a lot. Coming up with an optimization in the future will make this process more worth it
        float discriminant = pow(b, 2) - 4*a*c;
        if(discriminant <= 0) continue;
        //We always take the smaller one, even if it is negative, cause the larger one will always require the balls to phase thru each other.
        float new_T = std::min((-b + sqrt(discriminant))/(2*a), (-b - sqrt(discriminant))/(2*a)) + objects[ball].t;
        float bT = (objects[i].collision == nullptr) ? time : objects[i].collision->t;
        if(new_T < T && new_T < bT && new_T > objects[ball].t) {
            T = new_T;
            if(balls.size() == 1) balls.emplace_back(i);
            else balls[1] = i;
            type = 2;
        }
    }

    if(type != -1){
        event* collision_ptr = nullptr;
        int collision_indexer = -1;
        if(key_moments.find(T) == key_moments.end()) {
            key_moments.insert({T, {{0, {T, balls, {}, {}, type}}}});
            collision_ptr = &key_moments[T][0];
            collision_indexer = 0;
        } else {
            key_moments[T].emplace(key_moments[T].size(), event(T, balls, std::vector<t_vector>{}, std::vector<t_vector>{}, type));
            collision_indexer = key_moments[T].size() - 1;
            collision_ptr = &key_moments[T][collision_indexer];
        }
        if(objects[ball].collision != nullptr && objects[ball].collision->t > objects[ball].t){
            if(key_moments[objects[ball].collision->t].size() < 2) key_moments.erase(key_moments.find(objects[ball].collision->t));
            else key_moments[objects[ball].collision->t].erase(objects[ball].collision_index);
        }
        objects[balls[0]].collision = collision_ptr;
        objects[balls[0]].collision_index = collision_indexer;
    } else {
        objects[balls[0]].collision = nullptr;
        objects[balls[0]].collision_index = -1;
    }

    if(type == 2){
        if(objects[balls[1]].collision != nullptr && objects[balls[1]].collision->t > objects[balls[1]].t){
            if(objects[balls[1]].collision->axis == 2){
                bool index = (objects[balls[1]].collision->particles[0] == balls[1]) ? false : true;
                objects[objects[balls[1]].collision->particles[1 - index]].collision = nullptr;
                objects[objects[balls[1]].collision->particles[1 - index]].collision_index = -1;
                predict(objects[balls[1]].collision->particles[1 - index], {objects[balls[1]].collision->particles[index]});
            }
            if(key_moments[objects[balls[1]].collision->t].size() < 2) key_moments.erase(key_moments.find(objects[balls[1]].collision->t));
            else key_moments[objects[balls[1]].collision->t].erase(objects[balls[1]].collision_index);
        }
        objects[balls[1]].collision = objects[ball].collision;
    }
}

bool simulation::run(){
    CALL(glClear(GL_COLOR_BUFFER_BIT));
    std::unordered_set<int> exclusions = {};
    auto it = framed_moments.find(timer);
    if(it != framed_moments.end()){
        for(int i = 0; i < it->second.particles.size(); i++){
            exclusions.insert(it->second.particles[i]);
            objects[it->second.particles[i]].position = it->second.positions[i];
            objects[it->second.particles[i]].velocities[0] = it->second.velocities[i];
        }
    }
    for(int i = 0; i < objects.size(); i++){
        objects[i].render(true);
        if(exclusions.find(i) != exclusions.end()) {
            objects[i].translate(0, 0);
            continue;
        }
        objects[i].move();
    }
    timer++;
    if(time == timer) return false;
    return true;
}