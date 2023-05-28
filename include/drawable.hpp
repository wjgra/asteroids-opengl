#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

#include "../include/glad/glad.h"

//#include <SDL.h>
//#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

class Drawable{
public:
    Drawable();
    ~Drawable();
    void setUpBuffers();
private:
    std::vector<float> vertices;
    std::vector<GLuint> elements;
    GLuint VBO, EBO, VAO;
};

#endif