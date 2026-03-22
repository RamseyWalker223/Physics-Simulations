#include "simulation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void particle::move(float dt){
    this->time += dt;
    simulation::interpolate(dt, position, velocity);
}

bool particle::next_collision(float& timer, collision_type& type, bool& new_time){
    //Calculating which wall it will hit first is easier right now, but with accelleration that will become more difficult. And t will NEVER be less than objects[ball].time

    float reference = timer;

    bool collided = false;
    
    float dx = aspect - radius;
    
    if(velocity.x < 0) dx*=-1.0f;
    dx = dx - position.x;
    float tx = dx/velocity.x + time;
    if(tx <= timer && tx >= time){
        if(timer != tx) {
            new_time = true;
            timer = tx;
        }
        type = WALL;
        collided = true;
    }

    float dy = 1.0f - radius;
    
    if(velocity.y < 0) dy*=-1.0f;
    dy = dy - position.y;
    float ty = dy/velocity.y + time;
    if(ty <= timer && ty >= time){
        if(timer != ty) {
            new_time = true;
            timer = ty;
        } else {
            new_time = false;
        }
        type = CEILING;
        collided = true;
    }
    return collided;
}

particle_m::particle_m(float x, float y, float vx, float vy, float ax, float ay, float mass, float radius, std::string source, float aspect, float r, float g, float b, float a){
    this->ball.radius = radius;
    this->ball.mass = mass;
    this->ball.aspect = aspect;
    this->ball.position.x = x;
    this->ball.position.y = y;
    this->ball.velocity = {vx, vy};
    this->ball.time = 0.0f;


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
    program->setuniform2f("center", glm::vec2(x, y));
    program->setuniform4f("u_Color", glm::vec4(r, g, b, a));
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    program->setuniformMat4f("u_Matrix", projection);
}

void particle_m::update(){
    square.points = {
        (ball.position.x - ball.radius), ball.position.y - ball.radius,
        (ball.position.x + ball.radius), ball.position.y - ball.radius,
        (ball.position.x + ball.radius), ball.position.y + ball.radius,
        (ball.position.x - ball.radius), ball.position.y + ball.radius
    };
    vertex->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, square.points.size() * sizeof(float), square.points.data());
    program->bind();
    program->setuniform2f("center", glm::vec2(ball.position.x, ball.position.y));
    drawing.Draw(*array, *index, *program);
}

void particle_m::tick(){
    ball.position.x += ball.velocity.x;
    ball.position.y += ball.velocity.y;

    update();
}

