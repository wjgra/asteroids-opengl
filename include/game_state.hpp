#ifndef _ASTEROIDS_GAME_STATE_HPP_
#define _ASTEROIDS_GAME_STATE_HPP_

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include "../include/glad/glad.h"
#endif

#include <SDL.h>
#include <SDL_opengl.h>

#include <cmath>
#include <time.h>
#include <vector>

#include "../include/ship.hpp"
#include "../include/asteroid.hpp"
#include "../include/text_renderer.hpp"

#include <glm/gtc/type_ptr.hpp>

class GameState{
public:
    GameState(unsigned int width, unsigned int height);
    ~GameState();
    void frame(unsigned int frameTime);
    void handleEvents(SDL_Event const&  event);
private:   
    // Event handlers
    void handleEventsPlay(SDL_Event const&  event);
    void handleEventsPause(SDL_Event const&  event);
    void handleEventsScore(SDL_Event const&  event);
    void handleEventsMenu(SDL_Event const&  event);
    // Execute every frame
    void framePlay(unsigned int frameTime);
    void framePause(unsigned int frameTime);
    void frameScore(unsigned int frameTime);
    void frameMenu(unsigned int frameTime);
    // -- Collisions
    bool checkCollisionCoarse(const Asteroid& ast, const Missile& mis) const;
    bool checkCollisionCoarse(const Asteroid& ast, const Ship& sh) const;
    bool checkCollisionFine(const Asteroid& ast, const Missile& mis) const;
    bool checkCollisionFine(const Asteroid& ast, const Ship& sh) const;
    struct Triangle {float points[6];}; // struct Triangle {float x1, y1, x2, y2, x3, y3;};
    bool pointsLieOnOneSide(Triangle const& tri1, Triangle const& tri2) const;
    bool trianglesIntersect(Triangle const& tri1, Triangle const& tri2) const;
    // -- Drawing
    void drawShip() const;
    void drawMissiles();
    void drawAsteroids();
    // Utility functions
    void newGame(std::vector<unsigned int> const& astLayout  = std::vector<unsigned int>({2, 4, 6, 1})); // reset all game params
    void newWave();
public:
    enum class Screen {play, pause, score, menu, quit} screen;
    // Drawable game objects
    std::unique_ptr<Ship> ship;
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    // Text renderer
    TextRenderer textRen;
private:
    // Dimensions of notional window
    unsigned int const winWidth = 640;
    unsigned int const winHeight = 480;
    // Drawing variables
    float shipScale;
    // -- Uniform locations
    GLint uniformModelTrans;
    GLint uniformProjTrans;
    GLint uniformColour;
    // Shaders
    ShaderProgram wrapShader;
    // Simulation variables
    std::vector<int> initialAsteroidField;
    unsigned int wave = 0;
    int timeSinceLastCollCheck = 0;
    unsigned int score = 0;
};

#endif