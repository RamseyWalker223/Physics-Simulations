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
/*int main(){
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
    scene.set_sim_rand(200, 0.03f, 0.5f, 0.005f, 1.0f, 5.0f, 1000);
    scene.run(true, false);
    return 0;
}*/

//scene.set_sim_rand(200, 0.03f, 0.5f, 0.005f, 1.0f, 5.0f, 1000);
//This took 50 seconds without the optimization

//3d
int main(){
    std::vector<glm::vec3> positions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
    };

    std::vector<glm::vec4> colors = {
        {0.0f, 1.0f, 1.0f, 1.0f},
    };

    scene_3d scene(width, height, fps, positions, 0.2f, 2.5f, glm::radians(fov), "../res/textures/earth.png", colors, "../res/shaders/3d.shader", 500, 500, 1.0f);
    scene.run(false, true, true);
    return 0;
}