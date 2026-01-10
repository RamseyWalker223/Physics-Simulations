#include "renderer.h"
#include "shapes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <format>
#include "texture.h"

static const int width = 960;
static const int height = 540;
static float aspect = (float)width/(float)height;

int main(){

    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);


    window = glfwCreateWindow(width, height, "SIMULATION", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Error!\n";
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << "\n";

    //This should be put inside a renderer class another time.
    glEnable(GL_DEPTH_TEST);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    float points[40] = {    //texture coords
        -0.5, -0.5, 0.5,    0.0f, 0.0f,
        -0.5, 0.5,  0.5,    0.0f, 1.0f,
         0.5, 0.5,  0.5,    1.0f, 1.0f,
         0.5, -0.5, 0.5,    1.0f, 0.0f,

        -0.5, -0.5, -0.5,    1.0f, 0.0f,
        -0.5, 0.5,  -0.5,    1.0f, 1.0f,
         0.5, 0.5,  -0.5,    0.0f, 1.0f,
         0.5, -0.5, -0.5,    0.0f, 0.0f
    };

    int indices[36] = {
        0, 1, 2,
        0, 2, 3,

        3, 2, 6,
        3, 6, 7,

        0, 1, 5,
        0, 4, 5,

        4, 5, 6,
        4, 6, 7,

        1, 5, 6,
        1, 6, 2,

        0, 7, 3,
        0, 4, 7
    };

    glm::vec3 cubePositions[] = {
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


    v_buffer vb(sizeof(points) * sizeof(float), points);

    texture image("../res/textures/image.jpg");
    image.bind();

    v_array va;
    arrayLayout layout;
    layout.push(3, GL_FLOAT);
    layout.push(2, GL_FLOAT);
    va.addBuffer(vb, layout);

    i_buffer ib(sizeof(indices), indices);

    shader fubar("../res/shaders/3d.shader");
    fubar.bind();

    renderer scene;

    fubar.setuniformMat4f("projection", projection);

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    const float cameraSpeed = 0.025f;

    while(!glfwWindowShouldClose(window)){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            cameraPos += cameraSpeed * cameraUp;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
            cameraPos -= cameraSpeed * cameraUp;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            cameraPos += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        scene.Clear();
        //For now we put this outside, well put it inside the clear function later
        CALL(glClear(GL_DEPTH_BUFFER_BIT));

        fubar.setuniformMat4f("view", view);

        for(unsigned int i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            fubar.setuniformMat4f("model", model);
            scene.Draw(va, ib, fubar);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //particle ball(-1.00f + 0.1f, 0, 0, 0, 0.00f, 0.00f, 1.0f, 0.1f, "../res/shaders/circle.shader", aspect, 1.0f, 0.1f, 0.0f, 1.0f, 100);

    /*std::vector<float> balls = {
        //      x            y        vx       vy     ax     ay   mass  radius  r     g     b     a
        -1.00f + 0.1f, -1.00f + 0.1f, 0.03f, 0.025f, 0.00f, 0.00f, 1.0f, 0.1f, 1.0f, 0.1f, 0.0f, 1.0f,
        1.00f - 0.1f, -1.00f + 0.1f, -0.03f, 0.025f, 0.00f, 0.00f, 1.0f, 0.1f, 0.0f, 1.0f, 0.25f, 1.0f,
        0.0f, 0.0f, 0-.01f, -0.025f, 0.00f, 0.00f, 1.5f, 0.3f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.4, -0.4, 0.02f, -0.03f, 0.00f, 0.00f, 3.0f, 0.2f, 0.0f, 1.0f, 1.25f, 1.0f
    };

    simulation circles(balls, "../res/shaders/circle.shader", aspect, 1000);
    //simulation circles(9, 6, 1, "../res/shaders/circle.shader", aspect, 1000);

    std::string cmd_str = std::format("ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s {}x{} -i - -threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip ../videos/simulation.mp4 -pix_fmt yuv420p", width, height);

    const char* cmd = cmd_str.c_str();

    FILE* ffmpeg = popen(cmd, "w");
    int* buffer = new int[width*height];

    while (!glfwWindowShouldClose(window) && circles.run()){
        //

        //ball.render(false);
        //ball.move();

        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        fwrite(buffer, sizeof(int)*width*height, 1, ffmpeg);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    pclose(ffmpeg);*/
    return 0;
}