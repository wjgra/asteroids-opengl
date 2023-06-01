#include "../include/glad/glad.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
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
#include "../include/asteroid.hpp"
#include "../include/game_state.hpp"

static void handleEvents(SDL_Event event, GameState& gState);

int main(int argc, char* argv[]){
    // Initialise gamestate struct
    GameState gameState = {};
    gameState.winScale = 2;

    // Initialise SDL
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

    // Create window
    gameState.window = SDL_CreateWindow(
        "Asteroids", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        gameState.winScale*gameState.winWidth, 
        gameState.winScale*gameState.winHeight, 
        gameState.winFlags);

    if (!gameState.window){
        std::cout << "Failed to create window\n";
        return EXIT_FAILURE;
    }

    // Create OpenGL context
    gameState.context = SDL_GL_CreateContext(gameState.window);
    if (!gameState.context){
        std::cout << "Failed to create OpenGL context\n";
        return EXIT_FAILURE;
    }
    std::cout << "OpenGL loaded\n";

    // Load OpenGL functions with GLAD
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    // Display device information
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Set v-sync (to do: add toggle for v-sync?)
    SDL_GL_SetSwapInterval(gameState.useVsync);

    // Set viewport size (to do: allow resizable viewport?)
    glViewport(0, 0, gameState.winScale*gameState.winWidth, gameState.winScale*gameState.winHeight);
    
    // Create ship object at centre of screen, length 1/40th of screenwidth
    float shipScale = float(gameState.winWidth)/40;

    Ship ship(shipScale, gameState.winWidth/2.0f, gameState.winHeight/2.0f);
    gameState.ship = &ship; // Set pointer to ship object in gamestate

    Asteroid asteroid(shipScale*2.0f, gameState.winWidth/4.0f, gameState.winHeight/2.0f);

    // Compile shaders for ship
    ShaderProgram wrapShader(".//shaders//vertex.vert", ".//shaders//fragment.frag");
    wrapShader.useProgram();

    ship.setUpBuffers();
    asteroid.setUpBuffers();

    /*
    // Set up buffer objects for ship
    GLuint VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO); 

    // Bind VBO and copy vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, ship.vertices.size()*sizeof(float), ship.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind EBO and copy element data into EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ship.elements.size()*sizeof(GLuint), ship.elements.data(), GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
    */
    // Get locations of model/projection matrices
    GLint uniformModelTrans = wrapShader.getUniformLocation("model");
    GLint uniformProjTrans = wrapShader.getUniformLocation("projection");

    // Set orthogonal projection matrix
    glm::mat4 projection = glm::ortho(0.0f, (float)gameState.winWidth, (float)gameState.winHeight, 0.0f, -1.0f, 1.0f); 
    glUniformMatrix4fv(uniformProjTrans, 1, GL_FALSE, glm::value_ptr(projection));

    // Define offsets for instance-based screen wrap effect
    glm::vec2 instanceOffsets[9] = {
        glm::vec2(0.0f, 0.0f), // centre
        glm::vec2((float)gameState.winWidth, 0.0f), // E
        glm::vec2((float)gameState.winWidth, (float)gameState.winHeight), // NE
        glm::vec2(0.0f, (float)gameState.winHeight), // N
        glm::vec2(-(float)gameState.winWidth, (float)gameState.winHeight), // NW
        glm::vec2(-(float)gameState.winWidth, 0.0f), // W
        glm::vec2(-(float)gameState.winWidth, -(float)gameState.winHeight), // SW
        glm::vec2(0.0f , -(float)gameState.winHeight), // S
        glm::vec2((float)gameState.winWidth, -(float)gameState.winHeight) //SE
    };

    // Copy offsets to uniform variables
    for (int i = 0; i < 9; ++i){
        GLint offsetLocation = wrapShader.getUniformLocation("offsets["+std::to_string(i)+"]");
        glUniform2fv(offsetLocation,1,glm::value_ptr(instanceOffsets[i]));
    }    

    // Loop variables
    SDL_Event event;
    auto t_start = std::chrono::high_resolution_clock::now();
    float accumulatedFrameTime = 0.0f;
    unsigned int numFrames = 0;

    while (!gameState.quit){
        // Handle event queue
        while (SDL_PollEvent(&event)){
            handleEvents(event, gameState);
        }
        
        // Get duration of current frame in microseconds
        auto t_now = std::chrono::high_resolution_clock::now();
        unsigned int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(t_now-t_start).count();
 
        // Cap frame length at 250 ms in case of lag, thereby to
        // prevent simulation and display getting too out of sync
        if (frameTime > 250000){
            frameTime = 250000;
            // std::cout << "Frame hit maximum duration!\n";
        }

        // Calculate average frame length and FPS for display in title bar
        accumulatedFrameTime += frameTime;
        ++numFrames;
        if (accumulatedFrameTime > 1000000){ // every second
            float avgFrameTime = accumulatedFrameTime / ((float)numFrames*1000.0f); // in ms
            int FPS = int(1000.0f/avgFrameTime);
            SDL_SetWindowTitle(gameState.window, 
                std::string("Asteroids - FPS: "+std::to_string(FPS)+" ("+std::to_string(avgFrameTime)+" ms)").c_str());
            accumulatedFrameTime = 0;
            numFrames = 0;
        }
        t_start = t_now;

        // Clear buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Prepare to render
        wrapShader.useProgram();

        // Update ship transformation matrix
        glm::mat4 trans = ship.getTransMatrix(frameTime);
        glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));

        // Draw ship
        ship.draw();

        trans = asteroid.getTransMatrix(frameTime);
        glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
        asteroid.draw();

        //glDrawElementsInstanced(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0, 9);
        // Swap buffers
        SDL_GL_SwapWindow(gameState.window);
    }

    /*
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    */
    ship.releaseBuffers();
    asteroid.releaseBuffers();
    SDL_GL_DeleteContext(gameState.context);
    SDL_DestroyWindow(gameState.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

static void handleEvents(SDL_Event event, GameState& gState){
    switch(event.type){
        case SDL_QUIT:
            gState.quit = true;
            break;
        //case SDL_WINDOWEVENT: // To do: handle window resize events
            //break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_ESCAPE:
                    gState.quit = true;
                    break;
                case SDL_SCANCODE_F11:
                    gState.fullScreen = !gState.fullScreen;
                    if (gState.fullScreen){
                        SDL_SetWindowFullscreen(gState.window, gState.winFlags | SDL_WINDOW_FULLSCREEN);
                    }
                    else{
                        SDL_SetWindowFullscreen(gState.window, gState.winFlags);
                    }
                    break;
                case SDL_SCANCODE_LEFT:
                    gState.ship->turnLeft(true);
                    break;
                case SDL_SCANCODE_RIGHT:
                    gState.ship->turnRight(true);
                    break;
                case SDL_SCANCODE_UP:
                    gState.ship->thrustForward(true);
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_LEFT:
                    gState.ship->turnLeft(false);
                    break;
                case SDL_SCANCODE_RIGHT:
                    gState.ship->turnRight(false);
                    break;
                case SDL_SCANCODE_UP:
                    gState.ship->thrustForward(false);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}