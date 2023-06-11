#ifndef _ASTEROIDS_WINDOW_HPP_
#define _ASTEROIDS_WINDOW_HPP_

#include <SDL.h>
#include <SDL_opengl.h>

#include <string>

class Window{
public:
    Window(unsigned int width, unsigned int height);
    ~Window();
    SDL_Window* getWindow();
    void toggleFullScreen();
    void frame(unsigned int frameTime);
    unsigned int const winWidth = 640; // Not strictly needed (consider removing)
    unsigned int const winHeight = 480;
private:
    SDL_Window* window = nullptr;
    bool fullScreen = false;
    Uint32 winFlags = SDL_WINDOW_OPENGL;
    // FPS counter
    unsigned int accumulatedFrameTime = 0;
    unsigned int numFrames = 0;
};

#endif