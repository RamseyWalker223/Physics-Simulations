#include "renderer.h"
#include "vertexarray.h"

void arrayLayout::push(const int& count, const GLenum& type){
    switch(type){
        case GL_UNSIGNED_INT:
            stride+=4 * count;
            break;
        case GL_UNSIGNED_BYTE:
            stride+= 1 * count;
            break;
        case GL_FLOAT:
            stride+= 4 * count;
    }
    elements.push_back({count, type});
}

v_array::v_array(){
    glGenVertexArrays(1, &ID);
}

v_array::~v_array(){
    glDeleteVertexArrays(1, &ID);
}

void v_array::bind(){
    glBindVertexArray(ID);
}

void v_array::unbind(){
    glBindVertexArray(0);
}


void v_array::addBuffer(v_buffer& buffer, const arrayLayout& layout){
    bind();
    buffer.bind();
    unsigned int offset = 0;
    for(int i = 0; i < layout.elements.size(); i++){
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, layout.elements[i].count, layout.elements[i].type, GL_FALSE, layout.stride, (const void*)offset);
        offset+= layout.elements[i].count * layout.elements[i].type;
    }
}