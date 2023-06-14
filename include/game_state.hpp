#ifndef _ASTEROIDS_GAME_STATE_HPP_
#define _ASTEROIDS_GAME_STATE_HPP_

#include "../include/glad/glad.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <cmath>

#include "../include/ship.hpp"
#include "../include/asteroid.hpp"

#include <glm/gtc/type_ptr.hpp>

class GameState{
public:
    GameState(unsigned int width, unsigned int height);
    ~GameState();
    void frame(unsigned int frameTime);
    // Dimensions of notional window
    unsigned int const winWidth = 640;
    unsigned int const winHeight = 480;
    float shipScale;
    // Uniform locations
    GLint uniformModelTrans;
    GLint uniformProjTrans;
    GLint uniformColour;
    // Drawable game objects
    std::unique_ptr<Ship> ship;
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    // Shaders
    ShaderProgram wrapShader;
private:
    bool checkCollisionCoarse(const Asteroid& ast, const Missile& mis);
    int timeSinceLastCollCheck = 0; //consider unifying step forward across draw/coll check (better than passing pointers?)
};

#endif