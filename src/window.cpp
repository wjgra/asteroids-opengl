#include "../include/window.hpp"

Window::Window(unsigned int width, unsigned int height) : winWidth{width}, winHeight{height}{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        throw "Failed to initialise SDL";
    }

    // Can I move this to context class?
    SDL_GL_LoadLibrary(nullptr); 
    // Request an OpenGL 3.3 context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // End "can I move this?"

    // Create window
    window = SDL_CreateWindow(
        "Asteroids", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        winWidth, 
        winHeight, 
        winFlags);

    if (!window){
        throw "Failed to create window";
    }
}

Window::~Window(){
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Window* Window::getWindow(){
    return window;
}

void Window::toggleFullScreen(){
    fullScreen = !fullScreen;
    if (fullScreen){
        SDL_SetWindowFullscreen(window, winFlags | SDL_WINDOW_FULLSCREEN);
    }
    else{
        SDL_SetWindowFullscreen(window, winFlags);
    }
}