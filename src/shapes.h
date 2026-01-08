#pragma once
#include "renderer.h"
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <cfloat>
#include <map>


inline constexpr float ZERO = 1e-8f;

struct shape{
    std::vector<float> points;
    std::vector<int> indices;
};

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
    void interpolate(const float& time, const t_vector& v);
};

class simulation;
class event;

class particle{
    private:
        //Add critical velocity and position ten frames into the future to correct off of?
        //No idea if this would work.:/
        //Radius, position and initial velocity
        renderer drawing;
        std::unique_ptr<i_buffer> index;
        std::unique_ptr<v_buffer> vertex;
        std::unique_ptr<v_array> array;
        std::unique_ptr<shader> program;
        shape square;
        float time(float x1, float v, float a, float x2);
        float vf(float v, float a, float t);
        float d(float vf, float vi, float a);
        void collide(bool top, int axis);
    public:
        float radius, mass, t;
        event* collision;
        int collision_index;
        t_vector position, acceleration;
        t_vector velocities[2];
        particle(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, std::string source, float aspect, float r, float g, float b, float a);
        void render(bool multiple);
        void translate(float dx, float dy);
        void move();
        void place(float time);
        void interpolate(float& time);

};

//For now we will only store a pair of balls in a collision, but in the future, we might deside to make it a tree.
//If a ball hits a ball, and that ball is touching a bunch of other balls, thats a tree collision
//If a bunch of balls hit each other at the same time, thats a tree
//Not a binary tree tho
class event{
    public:
        float t;
        std::vector<int> particles;
        std::vector<t_vector> positions;
        std::vector<t_vector> velocities;
        //For axis, 0 is x and 1 is y and 2 is a collision with another particle
        //Im too lazy to think of a better data type
        int axis;
        event(float t, std::vector<int>& particles, const std::vector<t_vector>& positions, const std::vector<t_vector>& velocities, int axis);
        event(const std::vector<particle>& objects);
        event(const std::vector<event>& events);
        event(std::map<int, event>& events);
        void add(std::map<int, event>& events);
        event() = default;
};

class simulation{
    public:
        int time, timer;
        float aspect;
        std::vector<particle> objects;
        std::map<float, std::map<int, event>> key_moments;
        std::map<int, event> framed_moments;
        simulation(std::vector<float>& points, std::string source, float aspect, int time);
        simulation(int ball_X, int ball_Y, float mass_range, std::string source, float aspect, int time);
        void simulate();
        void predict(int ball, const std::unordered_set<int>& exceptions);
        void collide_wall(event& e);
        void collide(event& e);
        bool run();
};