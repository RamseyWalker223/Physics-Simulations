#include "renderer.h"
#include "vertexbuffer.h"

v_buffer::v_buffer(const int &size, const void* data){
    CALL(glGenBuffers(1, &ID));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
    CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

v_buffer::~v_buffer(){
    CALL(glDeleteBuffers(1, &ID));
}

void v_buffer::bind(){
    CALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void v_buffer::unbind(){
    CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void v_buffer::update(const int& size, const void* data){
    bind();
    CALL(glBufferSubData(GL_VERTEX_ARRAY, 0, size, data));
}