#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "../include/glad/glad.h"
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
//#include <algorithm>
  

class ShaderProgram
{
public:
    ShaderProgram(const std::string vertexPath, const std::string fragmentPath);
    ~ShaderProgram();
    GLuint getID() const;
    void useProgram();
    // Overloaded functions to set uniform values
    GLint getUniformLocation(const std::string &name) const;
private:
    GLuint programID;
    GLuint compileShader(const char *source, GLenum shaderType);
};
  
#endif