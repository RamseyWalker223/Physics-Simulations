#include "renderer.h"
#include "vertexarray.h"

int getTypeSize(const unsigned short& type){
    switch(type){
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        case GL_FLOAT:
            return 4;
    }
    std::cerr << "Invalid GLenum type!\n";
    return 69420;
}

void arrayLayout::push(const int& count, const unsigned short& type){
    stride += count * getTypeSize(type);
    elements.push_back({count, type});
}

v_array::v_array(){
    CALL(glGenVertexArrays(1, &ID));
}

v_array::~v_array(){
    CALL(glDeleteVertexArrays(1, &ID));
}

void v_array::bind(){
    CALL(glBindVertexArray(ID));
}

void v_array::unbind(){
    CALL(glBindVertexArray(0));
}


void v_array::addBuffer(v_buffer& buffer, const arrayLayout& layout){
    bind();
    buffer.bind();
    uintptr_t offset = 0;
    for(int i = 0; i < layout.elements.size(); i++){
        CALL(glEnableVertexAttribArray(i));
        CALL(glVertexAttribPointer(i, layout.elements[i].count, layout.elements[i].type, GL_FALSE, layout.stride, (const void*)offset));
        offset+= layout.elements[i].count * getTypeSize(layout.elements[i].type);
    }
}