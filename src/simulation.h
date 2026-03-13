#pragma once
#include "renderer.h"
#include <vector>
#include <set>
#include <list>
#include <stack>

enum collision_type{ NONE, WALL, CEILING, BALL };

struct t_vector{
    float x, y;
    float& operator[](int i){
        switch(i){
            case 0:
                return x;
            case 1:
                return y;
            default: throw std::out_of_range("oops.");
        }
    }
    //No acceleration for now, which makes interpolation really easy. 😎
    void interpolate(const float& time, const t_vector& v){
        this->x = v.x*time + this->x;
        this->y = v.y*time + this->y;
    }
};

struct shape{
    std::vector<float> points;
    std::vector<float> texture_coords;
    std::vector<int> indices;
};

struct change;
struct event;

struct particle{
    float radius, mass, time, aspect;
    t_vector position, velocity;
    std::optional<std::list<event>::iterator> collision;
    void move(float dt);
    event next_collision(float& time, int spot);
};

struct particle_m{
    public:
        renderer drawing;
        std::unique_ptr<i_buffer> index;
        std::unique_ptr<v_buffer> vertex;
        std::unique_ptr<v_array> array;
        std::unique_ptr<shader> program;
        shape square;
        particle ball;
        particle_m(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, std::string source, float aspect, float r, float g, float b, float a);
        void update();
        void tick();
        void render(bool multiple);
};

struct change{
    int ball;
    t_vector position;
    t_vector velocity;
    bool operator<(const change& other) const { return ball < other.ball; }
    friend bool operator<(const change& a, const int& b) { return a.ball < b; }
    friend bool operator<(const int& a, const change& b) { return a < b.ball; }
    change() = default;
    change(event& e, int index);
};

struct event{
    float* time;
    collision_type type = NONE;
    change balls[2];
    event(float& t, collision_type type, particle* foo, int spot){
        time = &t;
        this->type = type;
        balls[0].ball = spot;
        balls[0].position = foo->position;
        balls[0].velocity = foo->velocity;
        balls[0].position.interpolate(t - foo->time, foo->velocity);
    }
};

struct moment_f{
    float t;
    mutable std::list<event> events;
    bool operator<(const moment_f& other) const { return t < other.t; }
    friend bool operator<(const moment_f& a, const float& b) { return a.t < b; }
    friend bool operator<(const float& a, const moment_f& b) { return a < b.t; }
    moment_f(float& t);
    std::list<event>::iterator add_event(event& e) const;
};

struct moment_i{
    int t;
    mutable std::set<change, std::less<>> events;
    bool operator<(const moment_i& other) const { return t < other.t; }
    friend bool operator<(const moment_i& a, const int& b) { return a.t < b; }
    friend bool operator<(const int& a, const moment_i& b) { return a < b.t; }
    moment_i(int& t);
};

struct simulation{
    int time, timer;
    float aspect;
    std::stack<std::pair<int, std::set<int>>> queue;
    std::vector<particle> objects;
    std::vector<particle_m> objects_m;
    std::set<moment_f, std::less<>> key_moments;
    std::set<moment_i, std::less<>> framed_moments;
    simulation(std::vector<float>& points, std::string source, float aspect, int time);
    void simulate();
    void predict(const int& ball, const std::set<int>& exceptions);
    bool resolve(event& e);
    bool run();
};