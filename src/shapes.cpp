#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

circle::circle(float x, float y, float vx, float vy, float radius, std::string source, float aspect){
    rising = false;
    this->radius = radius;
    this->x = x * aspect;
    this->y = y;
    this->vx = vx * aspect;
    this->vy = vy;
    vxi = vx * aspect;
    vyi = vy;
    square.points = {
        x - radius, y - radius,
        x + radius, y - radius,
        x + radius, y + radius,
        x - radius, y + radius
    };
    square.indices = {
        0, 1, 2,
        2, 3, 0
    };
    vertex = std::make_unique<v_buffer>(square.points.size() * sizeof(float), square.points.data());
    index = std::make_unique<i_buffer>(square.indices.size(), square.indices.data());
    array = std::make_unique<v_array>();
    program = std::make_unique<shader>(source);
    arrayLayout layout;
    layout.push(2, GL_FLOAT);
    array->addBuffer(*vertex, layout);
    program->bind();
    program->setuniform1f("radius", radius);
    program->setuniform2f("center", x, y);
    program->setuniform4f("u_Color", 1.0f, 0.1f, 0.0f, 1.0f);
    glm::mat4 projection = glm::ortho(-1.0f*aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    program->setuniformMat4f("u_Matrix", projection);
}

void circle::render(){
    drawing.Clear();
    drawing.Draw(*array, *index, *program);
}

void circle::translate(float dx, float dy){
    x+=dx;
    y+=dy;

    square.points = {
        x - radius, y - radius,
        x + radius, y - radius,
        x + radius, y + radius,
        x - radius, y + radius
    };
    vertex->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, square.points.size() * sizeof(float), square.points.data());
    program->setuniform2f("center", x, y);
}

//Velocity and acceleration's unit of time is a frame
void circle::move(float ax, float ay, float aspect){
    //std::cout << "Position: " << y << "\n";
    //std::cout << "Velocity: " << vy << "\n";
    float prev_y = y;
    translate(vx, vy);
    

    /*if(y >= prev_y && !rising){
        rising = true;
    } else if(y < prev_y && rising){
        std::cout << "Max height:" << prev_y << "\n";
        rising = false;
    }*/

    vx+=ax;
    vy+=ay;
    //We assume perfect elasticity
    if(aspect - abs(x) <= radius) {
        if(x > 0.00f){
            vx = -vxi;
            x = aspect - radius;
        } else {
            vx = vxi;
            x = -aspect + radius;
        }
    }
    if(1.00f - abs(y) <= radius) {
        if(y > 0.00f){
            vy = -vyi;
            y = 1.00f - radius;
        } else {
            vy = vyi;
            y = -1.00f + radius;
        }
    }
}