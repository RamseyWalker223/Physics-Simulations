#include "renderer.h"
#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    circle ball(-1.00f + 0.06f, -1.0f + 0.06f, 0.01f, 0.03f, 0.06f, "../res/shaders/circle.shader", aspect);

    //For streaming, this doesnt really work
    //FILE* video = popen("ffmpeg -y -f rawvideo -pixel_format rgba -video_size 960x540 -framerate 60 -i - " "-vf vflip " "-pix_fmt yuv420p ../videos/simulation.mp4", "w");

    while (!glfwWindowShouldClose(window)){

        ball.render();
        
        //Doing this much acceleration seems to break it
        //Find out why
        ball.move(0.0000f, -0.0004f, aspect);

        //Outputing to video file
        //unsigned char frame[width*height*4];
        //glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, frame);
        //fwrite(frame, 1, sizeof(frame), video);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    //pclose(video);
    return 0;
}