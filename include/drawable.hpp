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
    Drawable(const std::vector<float>& verts, const std::vector<GLuint>& elts);
    ~Drawable();
    void setUpBuffers();
    void releaseBuffers();
    float const *getVertices() const;
    GLuint const *getElements() const;
    void draw();
//private:
    std::vector<float> const vertices;
    std::vector<GLuint> const elements;
    GLuint VBO, EBO, VAO;
};

#endif