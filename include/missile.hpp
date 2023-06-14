#ifndef _ASTEROIDS_MISSILE_HPP_
#define _ASTEROIDS_MISSILE_HPP_

#include "../include/glad/glad.h"
#include "../include/game_object.hpp"

class Missile : public GameObject{
public:
    Missile(float s, float pX, float pY, float dir, float shipVelX, float shipVelY);
    ~Missile();
    // Simulation functions
    // -- Called every frame
    virtual void beginFrame(unsigned int frameTime) override;
    virtual bool destroyThisFrame() override;
    // -- Called every simulation timestep
    virtual void updateNextPos() override;
private:
    unsigned int timeUntilDestroyed;
    float missileSpeed = 6.0f/10000.0f;
    unsigned int lifetime = 65 * timeStep;
};

#endif