#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

circle::circle(float x, float y, float vx, float vy, float radius, std::string source, float aspect){
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
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
    translate(vx, vy);
    vx+=ax;
    vy+=ay;
    if(1.000f * aspect - abs(x) <= radius) vx*=-1.00f;
    if(1.000f - abs(y) <= radius) vy*=-1.00f;
}