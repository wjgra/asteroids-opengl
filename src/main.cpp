#include "../include/glad/glad.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h> //only include in main.cpp
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

#include "../include/shader_program.hpp"
#include "../include/ship.hpp"

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

    

    GLuint elements[] = {0,1,2,3,0};

    ShaderProgram shipShader(".//shaders//vertex.vert", ".//shaders//fragment.frag");

    shipShader.useProgram();

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


    GLint uniformModelTrans = shipShader.getUniformLocation("model");
    GLint uniformProjTrans = shipShader.getUniformLocation("projection");

    glm::mat4 projection = glm::ortho(0.0f, (float)winWidth, (float)winHeight,0.0f, -1.0f, 1.0f); 


    Ship ship(shipScale, winWidth/2.0f, winHeight/2.0f);
    ship.updateNextPos();


    glm::vec3 shipPos = glm::vec3(winWidth/2.0f, winHeight/2.0f, 0.0f);

    auto t_start = std::chrono::high_resolution_clock::now();

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

        shipShader.useProgram();
        glBindVertexArray(VAO);
        //glDrawArrays(GL_LINE_STRIP, 0, 5);

        // rotation
       // Calculate transformation
       
       /* auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, shipPos);
        trans = glm::translate(trans, glm::vec3(0.5*shipScale, 0.5*shipScale, 0.0));
        trans = glm::rotate(trans, time * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(-0.5*shipScale, -0.5*shipScale, 0.0));
        trans = glm::scale(trans, glm::vec3(shipScale, shipScale, shipScale));
        */

        auto t_now = std::chrono::high_resolution_clock::now();

        // consider changing to float...
        unsigned int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(t_now-t_start).count();
        t_start = t_now;
        glm::mat4 trans = ship.getTransMatrix(frameTime);

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