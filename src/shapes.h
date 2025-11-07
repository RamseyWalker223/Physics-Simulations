#pragma once
#include "renderer.h"
#include <vector>

struct shape{
    std::vector<float> points;
    std::vector<int> indices;
};

class circle{
    private:
        //Add critical velocity and position ten frames into the future to correct off of?
        //No idea if this would work.:/
        //Radius, position and initial velocity
        float radius, x,y, vx, vy, vxi, vyi;
        bool rising;
        renderer drawing;
        std::unique_ptr<i_buffer> index;
        std::unique_ptr<v_buffer> vertex;
        std::unique_ptr<v_array> array;
        std::unique_ptr<shader> program;
        shape square;
    public:
        circle(float x, float y, float vx, float vy, float radius, std::string source, float aspect);
        void render();
        void translate(float dx, float dy);
        void move(float ax, float ay, float aspect);
};