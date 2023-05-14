#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h> //only include in main.cpp

#include "../include/glad/glad.h"

#include <SDL_opengl.h>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>

static void handleEvents(SDL_Window* window/*, SDL_Renderer* renderer*/){
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)){//keep window open until manually closed
            //SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
    } 
}

int main(int argc, char* argv[]){
    // Window dimensions
    const int winWidth = 640;
    const int winHeight = 480;

    SDL_Window* window = nullptr;
    //SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Failed to initialise SDL\n";
        return EXIT_FAILURE;
    }

    SDL_GL_LoadLibrary(nullptr);

    // Request an OpenGL 3.3 context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Uint32 winFlags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_FULLSCREEN;
    bool fullScreen = false;
    window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, winFlags);

    if (!window){
        std::cout << "Failed to create window\n";
        return EXIT_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context){
        std::cout << "Failed to create OpenGL context\n";
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL loaded\n";
    gladLoadGLLoader(SDL_GL_GetProcAddress);// This has to be after OpenGL is loaded!
    //renderer = SDL_CreateRenderer(window, -1, 0);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    while (true){
        //handleEvents(window);//, renderer);
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)){//keep window open until manually closed
                //SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                break;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_TAB){
            fullScreen = !fullScreen;
            if (fullScreen){
                SDL_SetWindowFullscreen(window, winFlags | SDL_WINDOW_FULLSCREEN);
            }
            else{
                SDL_SetWindowFullscreen(window, winFlags);
            }
        }
        
        SDL_GL_SetSwapInterval(1);
        glDisable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE);
       

        glViewport(0, 0, winWidth, winHeight);
        glClearColor(1.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}