#include "../include/shader_program.hpp"

// Ideally move file reading out of the constructor
ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath){
    std::string vertexSource, fragmentSource;
    std::ifstream vertexFile, fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();		

        vertexFile.close();
        fragmentFile.close();

        vertexSource   = vertexStream.str();
        fragmentSource = fragmentStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "Failed to load shader from file." << std::endl;
    }

    GLuint vertexShaderID = compileShader((const char*)vertexSource.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShaderID = compileShader((const char*)fragmentSource.c_str(), GL_FRAGMENT_SHADER);

    programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    
    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "Failed to link shader program.\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

GLuint ShaderProgram::compileShader(const char *source, GLenum shaderType){
    unsigned int shaderID;
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    // check for errors compiling shader...
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "Shader at " << source << " failed to compile.\n" << infoLog << std::endl;
    }
    return shaderID;
}

ShaderProgram::~ShaderProgram(){

}

GLuint ShaderProgram::getID() const{
    return programID;
}

void ShaderProgram::useProgram(){
    glUseProgram(programID);
}


// overload these instead
void ShaderProgram::setUniformBool(const std::string &name, bool value) const{
    
}

void ShaderProgram::setUniformInt(const std::string &name, int value) const{

}  

void ShaderProgram::setUniformFloat(const std::string &name, float value) const{

}