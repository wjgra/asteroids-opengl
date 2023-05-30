#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "../include/glad/glad.h"
#include "../include/drawable.hpp"

#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ship : public Drawable {
public:
    Ship(float s, float pX, float pY);
    ~Ship();
    // Simulation functions
    void updateNextPos();
    // -- Called every frame
    glm::mat4 getTransMatrix(unsigned int frameTime);
    // -- Called on user input
    void turnLeft(bool turn);
    void turnRight(bool turn);
    void thrustForward(bool thrust);
    // Utility functions
    void setOrientation(float orient);
    float getOrientation() const;
    void setVisibility(bool visibility);
    bool getVisibility() const;
public:
    // Simulation parameters
    float const drag = 5.0f / 10000000.0f;
    float const thrust = 2.4f / 10000000000.0f; // old value: drag*480.0f/(2000000.0f)
    float const timeStep = 15000; // in microseconds
    // N.B. Could consider allowing these to vary, but no reason to as of yet
private:
    float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation;
    float nextPosX, nextPosY, nextVelocityX, nextVelocityY;
    bool isVisible, isThrusting, isTurningLeft, isTurningRight;
    unsigned int timeSinceLastUpdate;  
};

#endif