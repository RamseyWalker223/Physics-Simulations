#include "renderer.h"
#include "shapes.h"

class cube{
    public:
        shape body;
        cube();
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
        cube mesh;
        std::vector<glm::vec3> positions;
        std::unique_ptr<camera> exposure;
        std::unique_ptr<v_buffer> vb;
        std::unique_ptr<i_buffer> ib;
        std::unique_ptr<shader> program;
        std::unique_ptr<texture> image;
        std::unique_ptr<v_array> va;
        std::unique_ptr<renderer> r;
        const char* cmd;
        FILE* ffmpeg;
        int* buffer;
        GLFWwindow* window;
    public:
        scene_3d(int& width, int& height, int& fps, std::vector<glm::vec3>& positions, std::vector<float>& scales, float sensitivity, float speed, float fov, std::string image, std::string program);
        void run(bool video, bool screen);
};