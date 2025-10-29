#pragma once

class i_buffer{
    private:
        unsigned int ID;
    public:
        unsigned int count;
        i_buffer(const int& count, const int* data);
        ~i_buffer();
        void bind();
        void unbind();
};