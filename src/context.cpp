#include "../include/context.hpp"

Context::Context(SDL_Window* window, unsigned int width, unsigned int height) : viewportWidth{width}, viewportHeight{height} {
    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    if (!context){
        throw "Failed to create OpenGL context";
    }
    std::cout << "OpenGL loaded\n";

    // Load OpenGL functions with GLAD
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    // Display device information
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Set v-sync
    SDL_GL_SetSwapInterval(useVsync);

    // Enable multisampling
    // glEnable(GL_MULTISAMPLE);

    // Set viewport size
    glViewport(0, 0, viewportWidth, viewportHeight);
}

Context::~Context(){
    SDL_GL_DeleteContext(context);
}

SDL_GLContext Context::getContext(){
    return context;
}