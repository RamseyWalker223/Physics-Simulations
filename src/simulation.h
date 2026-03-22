#pragma once
#include "renderer.h"
#include <vector>
#include <set>
#include <list>
#include <stack>

enum collision_type{ NONE, WALL, CEILING, BALL };

struct shape{
    std::vector<float> points;
    std::vector<float> texture_coords;
    std::vector<int> indices;
};

struct change;
struct event;

struct particle{
    float radius, mass, time, aspect;
    glm::vec2 position, velocity;
    std::optional<std::list<event>::iterator> collision;
    void move(float dt);
    //This boolean reference is true if its a brand new time, and false if the time doesnt change
    //The function returns false if no new collision was made and true if one was made.
    bool next_collision(float& time, collision_type& type, bool& new_time);
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
    glm::vec2 position;
    glm::vec2 velocity;
    bool operator<(const change& other) const { return ball < other.ball; }
    friend bool operator<(const change& a, const int& b) { return a.ball < b; }
    friend bool operator<(const int& a, const change& b) { return a < b.ball; }
    change() = default;
    change(event& e, int index, const float& time);
};

struct event{
    float* time;
    collision_type type = NONE;
    change balls[2];
    event(){
        time = nullptr;
    }
    event(collision_type type, int ballA, int ballB, float* t){
        this->type = type;
        balls[0].ball = ballA;
        balls[1].ball = ballB;
        time = t;
    }
};

struct moment_f{
    float t;
    std::list<event> events;
    moment_f() = default;
    void add_event(collision_type type, int ball_a, int ball_b);
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
    float aspect, sim_time;
    moment_f collision;
    std::vector<particle> objects;
    std::vector<particle_m> objects_m;
    std::set<moment_i, std::less<>> moments;
    simulation(std::vector<float>& points, std::string source, float aspect, int time);
    static void interpolate(const float& time, glm::vec2& p, const glm::vec2& v);
    void simulate();
    bool predict();
    void resolve(event& e);
    bool run();
};