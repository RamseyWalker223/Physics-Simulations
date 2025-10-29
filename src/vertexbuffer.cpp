#include "renderer.h"
#include "vertexbuffer.h"

v_buffer::v_buffer(const int &size, const void* data){
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

v_buffer::~v_buffer(){
    glDeleteBuffers(1, &ID);
}

void v_buffer::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void v_buffer::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}