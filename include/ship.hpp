#ifndef _ASTEROIDS_SHIP_HPP_
#define _ASTEROIDS_SHIP_HPP_

#include "../include/glad/glad.h"
#include "../include/game_object.hpp"
#include "../include/missile.hpp"

#include <vector>
#include <memory>

class Ship : public GameObject {
public:
    Ship(float s, float pX, float pY, float dir);
    ~Ship();
    // Simulation functions
    // -- Called every frame
    virtual void beginFrame(unsigned int frameTime) override;
    // -- Called every simulation timestep
    virtual void updateNextPos() override;
    void updateMissiles();
    // -- Called on user input
    void turnLeft(bool turn);
    void turnRight(bool turn);
    void thrustForward(bool thrust);
    void shootMissile(bool shooting);
    // Utility functions
    void setOrientation(float orient);
    float getOrientation() const;
    void setVisibility(bool visibility);
    bool getVisibility() const;
private:
    void spawnMissile();
public:
    // Simulation parameters
    float const drag = 5.0f / 10000000.0f;
    float const thrust = 2.4f / 10000000000.0f; // old value: drag*480.0f/(2000000.0f)
    unsigned int const minShootingInterval = 500000;
private:
    //float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation, 
      //  nextPosX, nextPosY, nextVelocityX, nextVelocityY;
    bool isVisible, isThrusting, isTurningLeft, isTurningRight, isShooting;
    unsigned int timeSinceLastShot;
public:
    std::vector<std::unique_ptr<Missile>> missiles;
};

#endif