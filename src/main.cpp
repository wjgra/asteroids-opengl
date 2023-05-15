#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h> //only include in main.cpp
#include "../include/glad/glad.h"
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

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

// Shaders
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main(int argc, char* argv[]){
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

    bool quit = false;
    SDL_Event event;

    float vertices[] = {
        -0.05f, 0.0f, 0.0f,
        0.05f, 0.05f, 0.0f,
        0.025f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f,
        -0.05f, 0.0f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// note last arg!

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // to do: element buffer object so 'tip' is not repeated; 

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
        glDrawArrays(GL_LINE_STRIP, 0, 5);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}