#include "../include/texture.hpp"

Texture::Texture(){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load("lucida_typewriter_font_2x.tga", &width, &height, &nrChannels, 0);
    // change to throw
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture(){
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}