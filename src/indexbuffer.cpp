#include "renderer.h"
#include "indexbuffer.h"

i_buffer::i_buffer(const int& count, const int* data){
    CALL(glGenBuffers(1, &ID));
    CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
    CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
    this->count = count;
}

i_buffer::~i_buffer(){
    CALL(glDeleteBuffers(1, &ID));
}

void i_buffer::bind(){
    CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void i_buffer::unbind(){
    CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}