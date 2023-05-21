#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "../include/glad/glad.h"
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();
    GLuint getID() const;
    void useProgram();
    void setUniformBool(const std::string &name, bool value) const;  
    void setUniformInt(const std::string &name, int value) const;   
    void setUniformFloat(const std::string &name, float value) const;
private:
    GLuint programID;
    GLuint compileShader(const char *source, GLenum shaderType);
};
  
#endif