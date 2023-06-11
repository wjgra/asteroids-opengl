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

#include "../include/app_state.hpp"

#include "../include/shader_program.hpp"
#include "../include/ship.hpp"
#include "../include/asteroid.hpp"
#include "../include/game_state.hpp"

static float const piValue = 3.1415926535897932385;

int main(int argc, char* argv[]){

    try{
        // Initialise window, openGL context and game state
        AppState appState(2);
        
        // Loop variables
        SDL_Event event;
        auto t_start = std::chrono::high_resolution_clock::now();
        float accumulatedFrameTime = 0.0f;
        unsigned int numFrames = 0;

        while (!appState.quit){
            // Handle event queue
            while (SDL_PollEvent(&event)){
                appState.handleEvents(event);
            }
            
            // Get duration of current frame in microseconds
            auto t_now = std::chrono::high_resolution_clock::now();
            unsigned int frameTime = std::chrono::duration_cast<std::chrono::microseconds>(t_now-t_start).count();
    
            // Cap frame length at 250 ms in case of lag, thereby to
            // prevent simulation and display getting too out of sync
            if (frameTime > 250000){
                frameTime = 250000;
                std::cout << "Frame hit maximum duration!\n";
            }

            
            t_start = t_now;

            appState.frame(frameTime);
        }

    }
    catch (const char* exception){
        // Exit if failed to initialise appState
        std::cerr << exception << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}