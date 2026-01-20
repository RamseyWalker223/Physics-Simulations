#include "2d.h"

scene_2d::scene_2d(const int& width, const int& height, const int& fps){
    this->width = width;
    this->height = height;
    this->aspect = (float)width/(float)height;
    this->fps = fps;


    //Opengl and window setup.
    if (!glfwInit())
        std::cerr << "Uh oh.\n";
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);


    window = glfwCreateWindow(width, height, "SIMULATION", NULL, NULL);


    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Error!\n";
    }

    std::cout << glGetString(GL_VERSION) << "\n";
}

void scene_2d::set_sim_man(std::vector<float>& balls, int time){
    this->world = std::make_unique<simulation>(balls, "../res/shaders/circle.shader", (float)this->width/(float)this->height, time);
}

void scene_2d::set_sim_fast(int ball_X, int ball_Y, float mass_Range, int time){
    this->world = std::make_unique<simulation>(ball_X, ball_Y, mass_Range, "../res/shaders/circle.shader", (float)this->width/(float)this->height, time);
}

void scene_2d::run(bool video, bool screen){

    if(video){
        std::string cmd_str = "ffmpeg -r " + std::to_string(fps) + " -f rawvideo -pix_fmt rgba -s " + std::to_string(width) + "x" + std::to_string(height) + " -i - -threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip ../videos/simulation.mp4 -pix_fmt yuv420p";
        this->cmd = cmd_str.c_str();

        this->ffmpeg = popen(cmd, "w");
        buffer = new int[width*height];
    }

    while(!glfwWindowShouldClose(window) && world->run()){

        if(video){
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            fwrite(buffer, sizeof(int)*width*height, 1, ffmpeg);
        }

        if(screen){
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    if(video) pclose(ffmpeg);
}