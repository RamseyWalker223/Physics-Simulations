#include "renderer.h"
#include "simulation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <format>
#include "texture.h"
#include "2d.h"
#include "3d.h"

int width = 960;
int height = 540;
int fps = 60;
float fov = 45.0f;

//2d
int main(){
    std::vector<float> balls = {
        //      x            y        vx       vy     ax     ay   mass  radius  r     g     b     a
        -1.00f + 0.1f, -1.00f + 0.1f, 0.03f, 0.025f, 0.00f, 0.00f, 1.0f, 0.1f, 1.0f, 0.1f, 0.0f, 1.0f,
        1.00f - 0.1f, -1.00f + 0.1f, -0.03f, 0.025f, 0.00f, 0.00f, 1.0f, 0.1f, 0.0f, 1.0f, 0.25f, 1.0f,
        0.0f, 0.0f, 0-.01f, -0.025f, 0.00f, 0.00f, 1.5f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.4, -0.4, 0.02f, -0.03f, 0.00f, 0.00f, 3.0f, 0.2f, 0.0f, 1.0f, 1.25f, 1.0f
    };

    scene_2d scene(width, height, fps);
    //scene.set_sim_man(balls, 100);
    //scene.set_sim_fast(25, 10, 0.8);
    scene.set_sim_rand(200, 0.01f, 0.75f, 0.00f, 0.01f, 100.0f, 1000);
    scene.run(true, true);
    return 0;
}

//3d
/*int main(){
    std::vector<glm::vec3> positions = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    std::vector<float> scales = {
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f, 
        0.5f
    };

    std::vector<glm::vec4> colors = {
    {1.00f, 0.00f, 0.00f, 1.0f},
    {1.00f, 0.50f, 0.00f, 1.0f},
    {1.00f, 1.00f, 0.00f, 1.0f},
    {0.00f, 1.00f, 0.00f, 1.0f},
    {0.00f, 0.00f, 1.00f, 1.0f},
    {0.29f, 0.00f, 0.51f, 1.0f},
    {0.58f, 0.00f, 0.83f, 1.0f},
    {0.75f, 0.00f, 1.00f, 1.0f},
    {0.00f, 1.00f, 1.00f, 1.0f},
    {1.00f, 0.00f, 1.00f, 1.0f},
};

    scene_3d scene(width, height, fps, positions, scales, 0.2f, 2.5f, glm::radians(fov), "../res/textures/dud.png", colors, "../res/shaders/3d.shader", 200, 200, 1.0f);
    scene.run(false, true, false);
    return 0;
}*/