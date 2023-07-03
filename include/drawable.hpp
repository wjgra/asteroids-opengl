#ifndef _ASTEROIDS_DRAWABLE_HPP_
#define _ASTEROIDS_DRAWABLE_HPP_

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include "../include/glad/glad.h"
#endif
#include "../include/shader_program.hpp"

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <vector>

class Drawable{
public:
    Drawable(const std::vector<float>& verts, const std::vector<GLuint>& elts/*, const glm::vec4& col*/);
    ~Drawable();
    void setUpBuffers();
    void releaseBuffers();
    float const *getVertices() const;
    GLuint const *getElements() const;
    void draw();
    void setDrawingMode(GLint mode);
//private:
    std::vector<float> const vertices;
    std::vector<GLuint> const elements;
    GLuint VBO, EBO, VAO;
    GLint drawingMode = GL_LINE_STRIP;
    //glm::vec4 lineColour;
    //GLint uniformLineColour;
};

#endif