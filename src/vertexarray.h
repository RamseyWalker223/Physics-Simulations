#pragma once
#include <vector>
#include "vertexbuffer.h"

struct bufferAttribs{
    int count;
    GLenum type;
};

class arrayLayout{
    public:
        std::vector<bufferAttribs> elements;
        int stride = 0;
        void push(const int& count, const unsigned short& type);
};

class v_array{
    private:
        unsigned int ID;
    public:
        v_array();
        ~v_array();
        void bind();
        void unbind();
        void addBuffer(v_buffer& buffer, const arrayLayout& layout);
};