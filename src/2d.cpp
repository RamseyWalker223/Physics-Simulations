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
    this->world = std::make_unique<simulation>(balls, "../res/shaders/circle.shader", aspect, time);
}

void scene_2d::set_sim_fast(int ballX, int ballY, float radius_shrinker){
    //Not perfect but whatever
    std::vector<float> balls = {};
    float spacing = std::min((aspect)*2/(float)ballX, 2.0f/(float)ballY);
    float radius = spacing*radius_shrinker/2.0f;
    for(int y = 0; y < ballY; y++){
        float y_pos = spacing*(float)y - 1.0f + spacing/2.0f;
        for(int x = 0; x < ballX; x++){
            float x_pos = spacing*(float)x - aspect + spacing/2.0f;
            std::list<float> elements = {x_pos, y_pos, 0.005f, 0.05f, 0.0f, 0.0f, 1.0f, radius, 1.0f, 0.1f, 0.0f, 1.0f};
            balls.insert(balls.end(), elements.begin(), elements.end());
        }
    }
    this->world = std::make_unique<simulation>(balls, "../res/shaders/circle.shader", aspect, 1000);
}

void scene_2d::set_sim_rand(int count, float min_radius, float max_radius, float max_speed, float min_mass, float max_mass, int time){
    std::vector<float> balls = {};
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> rand_radius(min_radius, max_radius);
    std::uniform_real_distribution<float> rand_speed(-max_speed, max_speed);
    std::uniform_real_distribution<float> rand_mass(min_mass, max_mass);
    std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);

    std::vector<std::tuple<float, float, float>> placed; // x, y, radius

    int attempts = 0;
    int placed_count = 0;
    while(placed_count < count && attempts < count * 100){
        attempts++;
        float r = rand_radius(rng);
        std::uniform_real_distribution<float> rand_x(-aspect + r, aspect - r);
        std::uniform_real_distribution<float> rand_y(-1.0f + r, 1.0f - r);
        float x = rand_x(rng);
        float y = rand_y(rng);

        bool overlap = false;
        for(auto& [ox, oy, or_] : placed){
            float dx = x - ox;
            float dy = y - oy;
            if(sqrt(dx*dx + dy*dy) < r + or_){
                overlap = true;
                break;
            }
        }
        if(overlap) continue;

        placed.emplace_back(x, y, r);
        float vx = rand_speed(rng);
        float vy = rand_speed(rng);
        float mass = rand_mass(rng);
        float red = rand_color(rng);
        float green = rand_color(rng);
        float blue = rand_color(rng);

        std::list<float> elements = {x, y, vx, vy, 0.0f, 0.0f, mass, r, red, green, blue, 1.0f};
        balls.insert(balls.end(), elements.begin(), elements.end());
        placed_count++;
    }
    this->world = std::make_unique<simulation>(balls, "../res/shaders/circle.shader", aspect, time);
}

void scene_2d::run(bool video, bool screen){
    world->simulate();
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