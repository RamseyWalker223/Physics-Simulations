#pragma once

class v_buffer{
    private:
        unsigned int ID;
    public:
        v_buffer(const int &size, const void* data);
        ~v_buffer();
        void bind();
        void unbind();
};