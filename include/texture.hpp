#ifndef _ASTEROIDS_TEXTURE_HPP_
#define _ASTEROIDS_TEXTURE_HPP_

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include "../include/glad/glad.h"
#endif

//#include <GL/gl.h>
//#include <GL/glu.h>

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