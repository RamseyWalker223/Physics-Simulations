#include "renderer.h"
#include "simulation.h"
#include <random>

class scene_2d{
    public:
        scene_2d(const int& width, const int& height, const int& fps);
        void set_sim_man(std::vector<float>& balls, int time);
        void set_sim_fast(int x, int y, float radius_shrinker);
        void set_sim_rand(int count, float min_radius, float max_radius, float max_speed, float min_mass, float max_mass, int time);
        void run(bool video, bool screen);
    private:
        GLFWwindow* window;
        int width, height, fps;
        float aspect;
        std::unique_ptr<simulation> world;
        const char* cmd;
        FILE* ffmpeg;
        int* buffer;
};