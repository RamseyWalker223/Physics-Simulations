#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include <iostream>
#include <cassert>

#define CALL(x) clearError();\
    x;\
    assert(printError(#x, __FILE__, __LINE__))

void clearError();

bool printError(const char* function, const char* file, int line);

class renderer{
    public:
        void Draw(v_array& va, i_buffer& ib, const shader& shader);
        void Clear();
};