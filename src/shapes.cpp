#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

circle::circle(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, std::string source, float aspect, float r, float g, float b, float a){
    rising = false;
    this->radius = radius;
    this->aspect = aspect;
    this->mass = mass;
    this->position.x = x;
    this->position.y = y;
    this->velocities[0] = {vx, vy};
    this->acceleration.x = ax;
    this->acceleration.y = ay;

    square.points = {
        (x - radius)*aspect, y - radius,
        (x + radius)*aspect, y - radius,
        (x + radius)*aspect, y + radius,
        (x - radius)*aspect, y + radius
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
    program->setuniform2f("center", x*aspect, y);
    program->setuniform4f("u_Color", r, g, b, a);
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    program->setuniformMat4f("u_Matrix", projection);
}

void circle::render(){
    //drawing.Clear();
    drawing.Draw(*array, *index, *program);
}

void circle::translate(float dx, float dy){
    position.x+=dx;
    position.y+=dy;

    square.points = {
        (position.x - radius)*aspect, position.y - radius,
        (position.x + radius)*aspect, position.y - radius,
        (position.x + radius)*aspect, position.y + radius,
        (position.x - radius)*aspect, position.y + radius
    };
    vertex->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, square.points.size() * sizeof(float), square.points.data());
    program->setuniform2f("center", position.x*aspect, position.y);
}

//Velocity and acceleration's unit of time is a frame
void circle::move(){
    velocities[0].x+=acceleration.x;
    velocities[0].y+=acceleration.y;

    translate(velocities[0].x, velocities[0].y);

    if(1.00f - abs(position.x) <= radius) {
        velocities[0].x = -velocities[0].x;
    }
    if(1.00f - abs(position.y) <= radius) {
        velocities[0].y = -velocities[0].y;
    }
}

float scene::distance(int first, int second){
    float x1, x2, y1, y2;
    x1 = items[first].position.x + items[first].aspect;
    x2 = items[second].position.x + items[second].aspect;
    y1 = items[first].position.y + 1.00f;
    y2 = items[second].position.y + 1.00f;
    if(x1 > x2) std::swap(x1, x2);
    if(y1 > y2) std::swap(y1, y2);
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

scene::scene(std::vector<float>& points, std::string source, float aspect){
    for(int i = 0; i < points.size(); i+=12){
        items.emplace_back(points[i], points[i + 1], points[i + 2], points[i + 3], points[i + 4], points[i + 5], points[i + 6], points[i + 7], source, aspect, points[i + 8], points[i + 9], points[i + 10], points[i + 11]);
    }
}

void scene::run(){
    CALL(glClear(GL_COLOR_BUFFER_BIT));
    for(int i = 0; i < items.size(); i++){
        items[i].render();
        if(i == 0 && distance(i, i + 1) <= items[i].radius + items[i + 1].radius){
            float x1, x2, y1, y2, θ;
            x1 = items[i].position.x;
            x2 = items[i + 1].position.x;
            y1 = items[i].position.y;
            y2 = items[i + 1].position.y;
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
            v1x = items[i].velocities[0].x;
            v1y = items[i].velocities[0].y;
            v2x = items[i + 1].velocities[0].x;
            v2y = items[i + 1].velocities[0].y;
            sinθ = sin(θ);
            cosθ = cos(θ);
            v1n = (v1x * cosθ) - (v1y * sinθ);
            v1t = (v1x * sinθ) + (v1y * cosθ);
            v2n = (v2x * cosθ) - (v2y * sinθ);
            v2t = (v2x * sinθ) + (v2y * cosθ);
            m1 = items[i].mass;
            m2 = items[i + 1].mass;
            masses = m1 + m2;
            v1nf = (v1n * (m1 - m2))/masses + (2 * m2 * v2n)/masses;
            v2nf = (v2n * (m2 - m1))/masses + (2 * m1 * v1n)/masses;
            sinθ = sin(-θ);
            cosθ = cos(-θ);
            items[i].velocities[0].x = (v1nf * cosθ) - (v1t * sinθ);
            items[i].velocities[0].y = (v1nf * sinθ) + (v1t * cosθ);
            items[i + 1].velocities[0].x = (v2nf * cosθ) - (v2t * sinθ);
            items[i + 1].velocities[0].y = (v2nf * sinθ) + (v2t * cosθ);
            //Output the total kinetic energy of the system.
            //std::cout << (pow(sqrt(pow(items[i].velocities[0].x, 2) + pow(items[i].velocities[0].y, 2)), 2) * items[i].mass) + (pow(sqrt(pow(items[i + 1].velocities[0].x, 2) + pow(items[i + 1].velocities[0].y, 2)), 2) * items[i + 1].mass) << "\n";
            items[i].move();
        } else {
            items[i].move();
        }
    }
}