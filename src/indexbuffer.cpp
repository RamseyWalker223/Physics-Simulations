#include "renderer.h"
#include "indexbuffer.h"

i_buffer::i_buffer(const int& count, const int* data){
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    this->count = count;
}

i_buffer::~i_buffer(){
    glDeleteBuffers(1, &ID);
}

void i_buffer::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void i_buffer::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}