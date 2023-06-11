#include "../include/app_state.hpp"

AppState::AppState(unsigned int scale) : 
    winScale{scale},
    window(winWidth*winScale, winHeight*winScale), 
    context(window.getWindow(), winWidth*winScale, winHeight*winScale),
    gameState(winWidth, winHeight)
{
}

AppState::~AppState(){
}

void AppState::handleEvents(SDL_Event const&  event){
    switch(event.type){
        case SDL_QUIT:
            quit = true;
            break;
        //case SDL_WINDOWEVENT: // To do: handle window resize events
            //break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_F11:
                    window.toggleFullScreen();
                    break;
                case SDL_SCANCODE_LEFT:
                    gameState.ship->turnLeft(true);
                    break;
                case SDL_SCANCODE_RIGHT:
                    gameState.ship->turnRight(true);
                    break;
                case SDL_SCANCODE_UP:
                    gameState.ship->thrustForward(true);
                    break;
                case SDL_SCANCODE_DOWN:
                    gameState.ship->shootMissile(true);
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_LEFT:
                    gameState.ship->turnLeft(false);
                    break;
                case SDL_SCANCODE_RIGHT:
                    gameState.ship->turnRight(false);
                    break;
                case SDL_SCANCODE_UP:
                    gameState.ship->thrustForward(false);
                    break;
                case SDL_SCANCODE_DOWN:
                    gameState.ship->shootMissile(false);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void AppState::frame(unsigned int frameTime){ // Move to app state frame()
    // Clear buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw game objects
    gameState.frame(frameTime);
    
    // Swap buffers
    SDL_GL_SwapWindow(window.getWindow());

    // Update FPS counter
    window.frame(frameTime);
}