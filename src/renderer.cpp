#include "renderer.h"

void renderer::Draw(v_array& va, i_buffer& ib, const shader& shader){
    shader.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.count, GL_UNSIGNED_INT, nullptr);
}

void renderer::Clear(){
    glClear(GL_COLOR_BUFFER_BIT);
}