#ifndef _ASTEROIDS_TEXT_RENDERER_HPP_
#define _ASTEROIDS_TEXT_RENDERER_HPP_

#include "../include/glad/glad.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/texture.hpp"
#include "../include/shader_program.hpp"
#include <glm/gtc/type_ptr.hpp>

class TextRenderer{
public:
    TextRenderer();
    ~TextRenderer();
    void setUpBuffers();
    void releaseBuffers();
    void draw();

    ShaderProgram shader;
    Texture texture;

    std::vector<float> const vertices {
        // positions   // texture coords
        0.5f, 0.5f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f,   0.0f, 1.0f    // top left 
    };

    std::vector<GLuint> const elements {1, 0, 3, 2};
    GLuint VBO, EBO, VAO;
    GLint drawingMode = GL_TRIANGLE_STRIP;

    GLint uniformModelTrans, uniformProjTrans;
};

#endif


