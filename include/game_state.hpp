#ifndef _GAME_STATE_HPP_
#define _GAME_STATE_HPP_

#include "../include/glad/glad.h"
#include "../include/ship.hpp"
#include <SDL.h>
#include <SDL_opengl.h>

struct GameState{
    // Window state variables
    SDL_Window* window;
    bool fullScreen;
    SDL_GLContext context;
    Uint32 winFlags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_RESIZABLE;
    bool useVsync = true;
    // -- Dimensions of notional window
    int const winWidth = 640;
    int const winHeight = 480;
    // -- Resolution is winScale * notional dimension
    unsigned int winScale;
    // Game state variables
    Ship* ship;
    bool quit = false;
};

#endif