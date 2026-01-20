#include "renderer.h"
#include "shapes.h"

class scene_2d{
    public:
        scene_2d(const int& width, const int& height, const int& fps);
        void set_sim_man(std::vector<float>& balls, int time);
        void set_sim_fast(int ball_X, int ball_Y, float mass_Range, int time);
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