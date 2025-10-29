#include "renderer.h"
#include "shapes.h"

int main(){
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OBAMA", NULL, NULL);
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

    shape tri;
    tri.points = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };

    tri.indices = {
        0, 1, 2
    };

    v_buffer vb(3 * 2 * sizeof(float), tri.points.data());

    i_buffer ib(3, tri.indices.data());

    arrayLayout layout;
    layout.push(2, GL_FLOAT);

    v_array va;
    va.addBuffer(vb, layout);

    shader nigga("../res/shaders/tri.shader");
    nigga.bind();

    nigga.setuniform4f("u_Color", 1.0f, 0.1f, 0.0f, 1.0f);

    renderer r;


    while (!glfwWindowShouldClose(window)){

        r.Clear();
        r.Draw(va, ib, nigga);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    return 0;
}