#ifndef _ASTEROIDS_APP_STATE_HPP_
#define _ASTEROIDS_APP_STATE_HPP_

#include "../include/glad/glad.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <vector>

#include "../include/window.hpp"
#include "../include/context.hpp"
#include "../include/shader_program.hpp"
#include "../include/game_state.hpp"

class AppState{
public:
    AppState(unsigned int scale);
    ~AppState();
    void handleEvents(SDL_Event const&  event);
    void frame(unsigned int frameTime);
    // Window/context state parameters
    bool quit = false;
    // -- Viewport resolution is winScale * notional dimension
    unsigned int const winScale = 1;
    // -- Dimensions of notional window
    int const winWidth = 640;
    int const winHeight = 480;
    // Ordering ensures context and shaders initialised before game objects and destroyed in reverse order
    Window window;
    Context context;
    GameState gameState;

};

#endif