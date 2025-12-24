#pragma once
#include "renderer.h"
#include <vector>
#include <cmath>

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
    
};

class circle{
    private:
        //Add critical velocity and position ten frames into the future to correct off of?
        //No idea if this would work.:/
        //Radius, position and initial velocity
        bool rising;
        float ty, tx;
        renderer drawing;
        std::unique_ptr<i_buffer> index;
        std::unique_ptr<v_buffer> vertex;
        std::unique_ptr<v_array> array;
        std::unique_ptr<shader> program;
        shape square;
    public:
        float radius, mass, aspect;
        t_vector position, acceleration;
        t_vector velocities[2];
        circle(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, std::string source, float aspect, float r, float g, float b, float a);
        void render();
        void translate(float dx, float dy);
        void move();
};

class scene{
    private:
        std::vector<circle> items;
    public:
        std::string source;
        scene(std::vector<float>& points, std::string source, float aspect);
        void push(float x, float y, float vx, float vy, float mass, float radius);
        void run();
        float distance(int first, int second);
};