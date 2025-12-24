#include "renderer.h"
#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

static const int width = 960;
static const int height = 540;
static float aspect = (float)width/(float)height;

int main(){


    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "SIMULATION", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Error!\n";
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << "\n";

    //circle ball(-1.00f + 0.06f, -1.0f + 0.06f, 0.01f, 0.03f, 0.0000f, -0.0004f, 0.0f, 0.06f, "../res/shaders/circle.shader", aspect, 1.0f, 0.1f, 0.0f, 1.0f);


    
    std::vector<float> balls = {
        //      x            y        vx       vy     ax     ay   mass  radius  r     g     b     a
        -1.00f + 0.2f, 1.00f - 0.2f, 0.021f, 0.015f, 0.00f, 0.00f, 1.0f, 0.1f, 1.0f, 0.1f, 0.0f, 1.0f,
        1.00f - 0.2f, -1.00f + 0.2f, -0.03f, 0.025f, 0.00f, 0.00f, 1.0f, 0.1f, 0.0f, 1.0f, 0.25f, 1.0f
    };

    const char* cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 960x540 -i - "
    "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip ../videos/output.mp4 -pix_fmt yuv420p";
    FILE* ffmpeg = popen(cmd, "w");
    int* buffer = new int[width*height];

    scene circles(balls, "../res/shaders/circle.shader", aspect);

    while (!glfwWindowShouldClose(window)){
        //usleep(500000);

        circles.run();

        //ball.render();
        //ball.move();

        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        fwrite(buffer, sizeof(int)*width*height, 1, ffmpeg);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    pclose(ffmpeg);
    return 0;
}