#include "renderer.h"
#include "shapes.h"

class scene_2d{
    public:
        scene_2d(const int& width, const int& height, const int& fps, std::vector<float>& points, int time);
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