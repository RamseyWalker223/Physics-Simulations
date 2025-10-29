#pragma once
#include <unordered_map>
#include <string>

struct shaderSource{
    std::string vertex;
    std::string fragment;
};

class shader{
    private:
        std::string filepath;
        unsigned int ID;
        int uniform_location(const std::string& name);
        std::unordered_map<std::string, int> location_cache;

        shaderSource parse(const std::string& file);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader,  const std::string& fragmentShader);
    public:
        shader(const std::string& filename);
        ~shader();

        void bind() const;
        void unbind() const;

        void setuniform1i(const std::string& name, int poo);
        void setuniform1f(const std::string& name, float value);
        void setuniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        //void setuniformMat4f(const std::string& name, const glm::mat4& matrix);
};