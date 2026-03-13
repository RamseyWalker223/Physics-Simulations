#include "renderer.h"
#include "simulation.h"

struct cube{
        shape body;
        cube();
};

struct sphere{
    shape body;
    float radius;
    void generate(int resX, int resY, float radius);
};

//Camera will also handle movement of the camera
class camera{
    private:
        float fov, pitch, yaw, last_x, last_y, sensitivity, speed, last_frame, Δtime;
        bool first = true;
    public:
        glm::vec3 pos, front, up;
        glm::mat4 view = glm::mat4(1.0f), projection;
        void mouse(GLFWwindow* window, double xpos, double ypos);
        void translate(GLFWwindow* window);
        camera(int& width, int& height, float& sensitivity, float& speed, float& fov);
};

class scene_3d{
    private:
        int width, height, fps;
        //cube mesh;
        sphere mesh;
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> positions;
        std::unique_ptr<camera> exposure;
        std::unique_ptr<v_buffer> vb;
        std::unique_ptr<i_buffer> ib;
        std::unique_ptr<shader> program;
        std::unique_ptr<texture> image;
        std::unique_ptr<v_array> va;
        std::unique_ptr<renderer> r;

        glm::vec3 light_pos;
        glm::vec4 light_color;

        const char* cmd;
        FILE* ffmpeg;
        int* buffer;
        GLFWwindow* window;
    public:
        scene_3d(int& width, int& height, int& fps, std::vector<glm::vec3>& positions, std::vector<float>& scales, float sensitivity, float speed, float fov, std::string image, std::vector<glm::vec4>& color, std::string program, int resX, int resY, float radius);
        void run(bool video, bool screen, bool texture);
};