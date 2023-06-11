#ifndef _GAME_STATE_HPP_
#define _GAME_STATE_HPP_

#include "../include/glad/glad.h"
#include "../include/ship.hpp"
#include "../include/asteroid.hpp"
#include "../include/context.hpp"

class GameState{
public:
    GameState(int scale);
    ~GameState();
    // Window state variables
    SDL_Window* window;
    bool fullScreen = false;
    SDL_GLContext context = nullptr;
    Uint32 winFlags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_RESIZABLE;
    bool useVsync = true;
    // -- Dimensions of notional window
    int const winWidth = 640;
    int const winHeight = 480;
    // -- Resolution is winScale * notional dimension
    unsigned int const winScale = 1;
    // Game state variables
    std::unique_ptr<Ship> ship;
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    bool quit = false;
};

GameState::GameState(int scale = 1) : winScale(scale){
    // Create ship object at centre of screen, length 1/40th of screenwidth
    float const shipScale = float(winWidth)/40;
    ship = std::make_unique<Ship>(shipScale, winWidth/2.0f, winHeight/2.0f);

    

    Ship ship();
}

GameState::~GameState(){
    ship->releaseBuffers();

    for (auto&& ast : asteroids)
    {
        ast->releaseBuffers();
    }
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif