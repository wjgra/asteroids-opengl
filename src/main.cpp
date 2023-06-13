//#include "../include/glad/glad.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>

#include <iostream>
#include <chrono>

#include "../include/app_state.hpp"

static float const piValue = 3.1415926535897932385;

int main(){

    try{
        // Initialise window, openGL context and game state
        AppState appState(2);
        
        // Loop variables
        SDL_Event event;
        auto t_start = std::chrono::high_resolution_clock::now();

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

// To do:
// - Rule of 3 for new classes
// - Move setupbuffers/releasebuffers to constructor/destructor