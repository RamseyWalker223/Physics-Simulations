#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "renderer.h"

shader::shader(const std::string& filename){
    filepath = filename;
    shaderSource source = parse(filepath);
    rendererID = CreateShader(source.vertex, source.geometry, source.fragment);
}

shader::~shader(){
    CALL(glDeleteProgram(rendererID));
}

shaderSource shader::parse(const std::string& file){
    std::ifstream input(file);

    enum class ShaderType{
        NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2
    };

    std::string line;
    std::stringstream files[3];
    ShaderType type = ShaderType::NONE;
    while(getline(input, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
                continue;
            }
            else if(line.find("geometry") != std::string::npos){
                type = ShaderType::GEOMETRY;
                continue;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
                continue;
            }
        }
        files[(int)type] << line << "\n";
    }
    return { files[0].str(), files[1].str(), files[2].str() };
}

unsigned int shader::CompileShader(unsigned int type, const std::string& source){
    CALL(unsigned int id = glCreateShader(type));
    const char* src = &source[0];
    //openGl requires a c-style string :)
    CALL(glShaderSource(id, 1, &src, nullptr));
    CALL(glCompileShader(id));


    //Here We are checking for errors in the compilation of the shader
    int result;
    CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if(result == GL_FALSE) {
        int length;
        CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* infolog = (char*)alloca(length * sizeof(char));

        CALL(glGetShaderInfoLog(id, length, &length, infolog));
        std::cout << "Failed to Compile:\n";
        std::cout << infolog << "\n";
        CALL(glDeleteShader(id));
        return 0;
    }
    

    return id;
}

unsigned int shader::CreateShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader){
    CALL(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    CALL(glAttachShader(program, vs));
    CALL(glAttachShader(program, gs));
    CALL(glAttachShader(program, fs));
    CALL(glLinkProgram(program));
    int result;
    CALL(glGetProgramiv(program, GL_LINK_STATUS, &result));
    if(result == GL_FALSE){
        int length;
        
        CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char* infolog = (char*)alloca(length * sizeof(char));
        
        CALL(glGetProgramInfoLog(program, length, &length, infolog));
        
        std::cout << "Failed to Link:\n";
        std::cout << infolog << "\n";
        CALL(glDeleteShader(program));
        return 0;
    }
    CALL(glValidateProgram(program));

    CALL(glDeleteShader(vs));
    CALL(glDeleteShader(gs));
    CALL(glDeleteShader(fs));

    return program;
}

void shader::bind() const{
    CALL(glUseProgram(rendererID));
}

void shader::unbind() const{
    CALL(glUseProgram(0));
}

int shader::uniform_location(const std::string& name){
    if(location_cache.find(name) != location_cache.end()){
        return location_cache[name];
    }
    CALL(int location = glGetUniformLocation(rendererID, name.c_str()));
    if(location == -1){
        std::cerr << "Uniform " << name << " doesnt exist!\n";
    } else {
        location_cache[name] = location;
    }
    return location;
}

void shader::setuniform1i(const std::string& name, int value){
    CALL(glUniform1i(uniform_location(name), value));
}
        

void shader::setuniform1f(const std::string& name, float value){
    CALL(glUniform1f(uniform_location(name), value));
}

void shader::setuniform2f(const std::string& name, const glm::vec2& v){
    CALL(glUniform2f(uniform_location(name), v.x, v.y));
}

void shader::setuniform3f(const std::string& name, const glm::vec3& v){
    CALL(glUniform3f(uniform_location(name), v.x, v.y, v.z));
}

void shader::setuniform4f(const std::string& name, const glm::vec4& v){
    CALL(glUniform4f(uniform_location(name), v.x, v.y, v.z, v.w));
}

void shader::setuniformMat4f(const std::string& name, const glm::mat4& matrix){
    CALL(glUniformMatrix4fv(uniform_location(name), 1, GL_FALSE, &matrix[0][0]));
}