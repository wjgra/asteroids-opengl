#ifndef _ASTEROIDS_TEXTURE_HPP_
#define _ASTEROIDS_TEXTURE_HPP_

#include "../include/glad/glad.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "../include/stb_image.h"

#include <iostream>

class Texture{
public:
    Texture();
    ~Texture();
    void bind();
    void unbind();
private:
    GLuint texture;
    GLint width, height, nrChannels;
};

#endif