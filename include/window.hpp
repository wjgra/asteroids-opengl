#ifndef _ASTEROIDS_WINDOW_HPP_
#define _ASTEROIDS_WINDOW_HPP_

#include <SDL.h>
#include <SDL_opengl.h>

class Window{
public:
    Window(unsigned int width, unsigned int height);
    ~Window();
    SDL_Window* getWindow();
    void toggleFullScreen();
    unsigned int const winWidth = 640; // Not strictly needed...
    unsigned int const winHeight = 480;
private:
    SDL_Window* window = nullptr;
    bool fullScreen = false;
    Uint32 winFlags = SDL_WINDOW_OPENGL;
};

#endif