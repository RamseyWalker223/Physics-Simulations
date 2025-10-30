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
    window = glfwCreateWindow(width, height, "OBAMA", NULL, NULL);
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

    circle ball(0.0f, 0.8f, 0.01f, -0.008f, 0.1f, "../res/shaders/circle.shader", aspect);

    while (!glfwWindowShouldClose(window)){

        ball.render();
        ball.move(0.0000f, 0.00f, aspect);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    return 0;
}