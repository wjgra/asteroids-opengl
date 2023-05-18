#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h> //only include in main.cpp
#include "../include/glad/glad.h"
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif

static bool handleEvents(SDL_Event event, SDL_Window* window){
    switch(event.type){
        case SDL_QUIT:
            return true;
            break;
        case SDL_WINDOWEVENT:
            /*if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                std::cout << "Resizing window\n";
                //SDL_SetWindowSize(window, event.window.data1, event.window.data2);
            }*/
            break;
        default:
            break;
    }
    
    // || (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)){// keep window open until manually closed with Esc
    
    // Return false if input does not trigger quit
    return false;
}

std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

// Shaders
/*
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"uniform mat4 trans;\n"
"void main()\n"
"{\n"
" gl_Position = trans * vec4(aPos.x, aPos.y, 0.0f, 1.0f);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(0.5f, 0.6f, 1.0f, 1.0f);\n"
"}\0";
*/

int main(int argc, char* argv[]){
    auto t_start = std::chrono::high_resolution_clock::now();
    // Window dimensions
    const int winWidth = 640;
    const int winHeight = 480;
    

    SDL_Window* window = nullptr;
    //SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Failed to initialise SDL\n";
        return EXIT_FAILURE;
    }

    SDL_GL_LoadLibrary(nullptr);

    // Request an OpenGL 3.3 context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Uint32 winFlags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_RESIZABLE;
    bool fullScreen = false;
    window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, winFlags);

    if (!window){
        std::cout << "Failed to create window\n";
        return EXIT_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context){
        std::cout << "Failed to create OpenGL context\n";
        return EXIT_FAILURE;
    }
    std::cout << "OpenGL loaded\n";
    gladLoadGLLoader(SDL_GL_GetProcAddress);// This has to be after OpenGL is loaded!
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    bool useVsync = true;
    SDL_GL_SetSwapInterval(useVsync);

    // Set viewport size
    glViewport(0, 0, winWidth, winHeight);

    //float aspectRatio = float(winWidth)/float(winHeight);
    


    bool quit = false;
    SDL_Event event;

    float vertices[] = {
        1.0f, 0.5f,  // Front of ship
        0.0f, 1.0f, // Rear right
        0.25f, 0.5f, // Rear centre
        0.0f, 0.0f // Rear left
    };
    
    float shipScale = float(winWidth)/40;

    /*
    // covert to screen space coords
    for (int i = 0; i < 4; ++i){
        vertices[2*i] = winWidth*(vertices[2*i]+1)/2; //x coord
        vertices[2*i+1] = winHeight*(vertices[2*i+1]+1)/2; //y coord
    }*/
    

    GLuint elements[] = {0,1,2,3,0};


    std::string vertShaderStr = readFile(".//shaders//vertex.vert");
    std::string fragShaderStr = readFile(".//shaders//fragment.frag");

    const char *vertexShaderSource = vertShaderStr.c_str();
    const char *fragmentShaderSource = fragShaderStr.c_str();


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for errors compiling shader...
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader failed to compile\n" <<
        infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for errors compiling shader...
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader failed to compile\n" <<
        infoLog << std::endl;
    }

    // Link shaders into shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //check for errors...
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program failed to compile\n" <<
        infoLog << std::endl;
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO after VAO!
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    GLint uniformModelTrans = glGetUniformLocation(shaderProgram, "model");
    GLint uniformProjTrans = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 projection = glm::ortho(0.0f, (float)winWidth, (float)winHeight,0.0f, -1.0f, 1.0f); 

    glm::vec3 shipPos = glm::vec3(winWidth/2.0f, winHeight/2.0f, 0.0f);

    while (!quit){
        
        // Handle event queue
        while (SDL_PollEvent(&event)){
            quit = handleEvents(event, window);
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_TAB){
                fullScreen = !fullScreen;
                if (fullScreen){
                    SDL_SetWindowFullscreen(window, winFlags | SDL_WINDOW_FULLSCREEN);
                }
                else{
                    SDL_SetWindowFullscreen(window, winFlags);
                }
            }
        }

        // Render
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_LINE_STRIP, 0, 5);

        // rotation
       // Calculate transformation
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, shipPos);
        trans = glm::translate(trans, glm::vec3(0.5*shipScale, 0.5*shipScale, 0.0));
        trans = glm::rotate(trans, time * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(-0.5*shipScale, -0.5*shipScale, 0.0));
        trans = glm::scale(trans, glm::vec3(shipScale, shipScale, shipScale));

        glUniformMatrix4fv(uniformProjTrans, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));


        glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}