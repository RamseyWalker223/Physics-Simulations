#define STB_IMAGE_IMPLEMENTATION
#include "../res/include/stb_image.h"
#include "texture.h"

texture::texture(const std::string& path){
    rendererID = 0;
    filepath = path;
    localbuffer = nullptr;
    width = 0;
    height = 0;
    bpp = 0;
    CALL(glGenTextures(1, &rendererID));
    CALL(glBindTexture(GL_TEXTURE_2D, rendererID));

    stbi_set_flip_vertically_on_load(1);
    localbuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localbuffer));
    CALL(glBindTexture(GL_TEXTURE_2D, 0));

    if(localbuffer){
        stbi_image_free(localbuffer);
    }
}

texture::~texture(){
    CALL(glDeleteTextures(1, &rendererID));
}

void texture::bind(unsigned int slot) const{
    CALL(glActiveTexture(GL_TEXTURE0 + slot));
    CALL(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void texture::unbind() const{
    CALL(glBindTexture(GL_TEXTURE_2D, 0));
}