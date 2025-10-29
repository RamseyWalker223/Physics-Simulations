#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include <iostream>

class renderer{
    public:
        void Draw(v_array& va, i_buffer& ib, const shader& shader);
        void Clear();
};