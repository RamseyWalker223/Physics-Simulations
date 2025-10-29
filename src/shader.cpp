#include "renderer.h"
#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

shader::shader(const std::string& filename){
    filepath = filename;
    shaderSource source = parse(filepath);
    ID = CreateShader(source.vertex, source.fragment);
}

shader::~shader(){
    glDeleteProgram(ID);
}

shaderSource shader::parse(const std::string& file){
    std::ifstream input(file);

    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream files[2];
    ShaderType type = ShaderType::NONE;
    while(getline(input, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
                continue;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
                continue;
            }
        }
        files[(int)type] << line << "\n";
    }
    return { files[0].str(), files[1].str() };
}

unsigned int shader::CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    //openGl requires a c-style string :)
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    //Here We are checking for errors in the compilation of the shader
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* infolog = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, infolog);
        std::cout << "Failed to Compile:\n";
        std::cout << infolog << "\n";
        glDeleteShader(id);
        return 0;
    }
    

    return id;
}

unsigned int shader::CreateShader(const std::string& vertexShader,  const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void shader::bind() const{
    glUseProgram(ID);
}

void shader::unbind() const{
    glUseProgram(0);
}

int shader::uniform_location(const std::string& name){
    if(location_cache.find(name) != location_cache.end()){
        return location_cache[name];
    }
    int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1){
        std::cerr << "Uniform " << name << " doesnt exist!\n";
    } else {
        location_cache[name] = location;
    }
    return location;
}

void shader::setuniform1i(const std::string& name, int value){
    glUniform1i(uniform_location(name), value);
}
        

void shader::setuniform1f(const std::string& name, float value){
    glUniform1f(uniform_location(name), value);
}

void shader::setuniform4f(const std::string& name, float v0, float v1, float v2, float v3){
    glUniform4f(uniform_location(name), v0, v1, v2, v3);
}