#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "../include/glad/glad.h"
#include "../include/ship.hpp"
#include <SDL.h>

class GameState{
public:
    SDL_Window* window;
    bool fullScreen;
    Ship* ship;
};

#endif