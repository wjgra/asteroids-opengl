#ifndef _ASTEROIDS_MISSILE_HPP_
#define _ASTEROIDS_MISSILE_HPP_

#include "../include/glad/glad.h"
#include "../include/drawable.hpp"

#include <chrono>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Missile : public Drawable{
public:
    Missile(float s, float pX, float pY, float dir, float shipVelX, float shipVelY);
    ~Missile();
    // Simulation functions
    void updateNextPos();
    // -- Called every frame
    glm::mat4 getTransMatrix(unsigned int frameTime);
    bool destroyThisFrame();
//private:
    float scale, posX, posY, velocityX, velocityY, nextPosX, nextPosY, orientation;
    unsigned int timeSinceLastUpdate;
    unsigned int shotDuration, timeUntilDestroyed;
    float missileSpeed = 6.0f/10000.0f;
    unsigned int timeStep = 15000; // in microseconds
    unsigned int lifetime = 65 * timeStep;
    bool toBeDestroyed;
};

#endif