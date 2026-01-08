#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

particle::particle(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, std::string source, float aspect, float r, float g, float b, float a){
    this->t = 0;
    this->collision = nullptr;
    this->collision_index = -1;
    this->radius = radius;
    this->mass = mass;
    this->position.x = x;
    this->position.y = y;
    this->velocities[0] = {vx, vy};
    this->acceleration.x = ax;
    this->acceleration.y = ay;

    square.points = {
        (x - radius), y - radius,
        (x + radius), y - radius,
        (x + radius), y + radius,
        (x - radius), y + radius
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
    program->setuniform4f("u_Color", r, g, b, a);
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    program->setuniformMat4f("u_Matrix", projection);
}

void particle::render(bool multiple){
    if(!multiple) drawing.Clear();
    drawing.Draw(*array, *index, *program);
}

void particle::translate(float dx, float dy){
    position.x+=dx;
    position.y+=dy;

    square.points = {
        (position.x - radius), position.y - radius,
        (position.x + radius), position.y - radius,
        (position.x + radius), position.y + radius,
        (position.x - radius), position.y + radius
    };
    vertex->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, square.points.size() * sizeof(float), square.points.data());
    program->setuniform2f("center", position.x, position.y);
}

float particle::time(float x, float v, float a, float x2){
    if(a == 0.00f){
        return (x2 - x)/v;
    }
    float discriminant = v*v - (2 * a * (x - x2));
    if(discriminant < 0.00f) return -1;
    else if(discriminant == 0.00f) return v/a;
    else {
        discriminant = sqrt(discriminant);
        //optimize the /a operator to be done once.
        return std::max((discriminant - v)/a, (-v - discriminant)/a);
    }
}

float particle::vf(float v, float a, float t){
    return v + a*t;
}

float particle::d(float vf, float vi, float a){
    return (pow(vf, 2) - pow(vi, 2))/(2*a);
}

void particle::collide(bool top, int axis){
    float wall = (top) ? 1.00f - radius : radius - 1.00f;
    float t = time(position[axis], velocities[0][axis], acceleration[axis], wall);
    float vi = -(vf(velocities[0][axis], acceleration[axis], t));
    velocities[0][axis] = vf(vi, acceleration[axis], 1.00f - t);
    if(acceleration[axis] == 0) position[axis] = (top) ? wall - abs(vi*(1.00f - t)) : wall + abs(vi*(1.00f - t));
    else position[axis] = (top) ? wall - abs(d(velocities[0][axis], vi, acceleration[axis])) : wall + abs(d(velocities[0][axis], vi, acceleration[axis]));
}

//Velocity and acceleration's unit of time is a frame
void particle::move(){
    velocities[0].x+=acceleration.x;
    velocities[0].y+=acceleration.y;

    translate(velocities[0].x, velocities[0].y);

    /*if(position.x + radius > 1.00f){
        position.x = position.x - velocities[0].x;
        collide(true, 0);
        translate(0.00f, 0.00f);
    } else if(position.x - radius < -1.00f){
        position.x = position.x - velocities[0].x;
        collide(false, 0);
        translate(0.00f, 0.00f);
    }

    if(position.y + radius > 1.00f){
        position.y = position.y - velocities[0].y;
        collide(true, 1);
        translate(0.00f, 0.00f);
    } else if(position.y - radius < -1.00f){
        position.y = position.y - velocities[0].y;
        collide(false, 1);
        translate(0.00f, 0.00f);
    }*/

    /*if(1.00f - abs(position.x) <= radius) {
        velocities[0].x = -velocities[0].x;
    }
    if(1.00f - abs(position.y) <= radius) {
        velocities[0].y = -velocities[0].y;
    }*/
}