#ifndef _ASTEROIDS_TEXT_RENDERER_HPP_
#define _ASTEROIDS_TEXT_RENDERER_HPP_

#include "../include/glad/glad.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <vector>
#include <string>

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
    //void draw();
    void setChar(char toDraw);
    void setPosition(float scale, float xPos, float yPos);
    void drawString(std::string toDraw, float scale, float xPos, float yPos);
    void drawStringCentred(std::string toDraw, float scale, float xPos, float yPos);
    ShaderProgram shader;
    Texture texture;

    std::vector<float> const vertices {
        // positions   // texture coords
        /*0.0625f, -0.0625f,   0.125f, 0.0f,   
        0.0625f, 0.0625f,   0.125f, 0.125f,   
        -0.0625f, -0.0625f,   0.0f, 0.0f,   
        -0.0625f, 0.0625f,   0.0f, 0.125f   */
        1.0f, 0.0f,   0.125f, 0.0f,   
        1.0f, 1.0f,   0.125f, 0.125f,   
        0.0f, 0.0f,   0.0f, 0.0f,   
        0.0f, 1.0f,   0.0f, 0.125f  

    };

    std::vector<GLuint> const elements {0, 1, 2, 3};
    GLuint VBO, EBO, VAO;
    GLint drawingMode = GL_TRIANGLE_STRIP;

    GLint uniformModelTrans, uniformProjTrans, uniformTextureCoordOffset;

};

#endif


