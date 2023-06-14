#include "../include/missile.hpp"

static float const piValue = 3.1415926535897932385;

Missile::Missile(float s, float pX, float pY, float dir, float shipVelX, float shipVelY): 
    GameObject({
        0.25f, 0.0f,  // front of missile
        0.0f, 0.25f,
        -0.25f, 0.0f,
        0.0f, -0.25f // back of missile
    }, 
    {0, 1, 3, 2}, s, pX, pY, dir){
    setDrawingMode(GL_TRIANGLE_STRIP);
    velocityX = shipVelX + missileSpeed*cos(dir);
    velocityY = shipVelY + missileSpeed*sin(dir);
    timeUntilDestroyed = lifetime;
    updateNextPos();
}

Missile::~Missile(){
}

void Missile::beginFrame(unsigned int frameTime){
    if (frameTime >= timeUntilDestroyed){
        toBeDestroyed = true;
    }
    else{
        timeUntilDestroyed -= frameTime;
    }
}

bool Missile::destroyThisFrame(){
    return toBeDestroyed;
}

// Updates the orientation and nextPos/nextVelocity variables by stepping forwards one timeStep.
void Missile::updateNextPos(){
    nextPosX = posX + timeStep * velocityX;
    nextPosY = posY + timeStep * velocityY;

    nextVelocityX = velocityX;
    nextVelocityY = velocityY;

    nextOrientation = orientation;
}