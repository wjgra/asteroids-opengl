#ifndef _ASTEROIDS_CONTEXT_HPP_
#define _ASTEROIDS_CONTEXT_HPP_

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include "../include/glad/glad.h"
#endif

#include <iostream>

#include <SDL.h>
#include <SDL_opengl.h>

class Context{
public:
    Context(SDL_Window* window, unsigned int width, unsigned int height);
    ~Context();
    SDL_GLContext getContext();
    bool const useVsync = true;
    // Dimensions of OpenGL viewport
    unsigned int const viewportWidth = 640;
    unsigned int const viewportHeight = 480;
private:
    SDL_GLContext context = nullptr;
    
};

#endif