#include "renderer.h"

void clearError(){
    while(glGetError());
}

bool printError(const char* function, const char* file, int line){
    while(GLenum error = glGetError()){
        std::cerr << "OpenGL Error: [" << error << "] Inside " << function << " " << file << ":" << line << "\n";
        return false;
    }
    return true;
}


void renderer::Draw(v_array& va, i_buffer& ib, const shader& shader){
    shader.bind();
    va.bind();
    ib.bind();
    CALL(glDrawElements(GL_TRIANGLES, ib.count, GL_UNSIGNED_INT, nullptr));
}

void renderer::Clear(){
    CALL(glClear(GL_COLOR_BUFFER_BIT));
}