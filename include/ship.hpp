#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "../include/glad/glad.h"

#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Ship{
public:
    Ship(float s, float pX, float pY);
    ~Ship();
    void changeOrientation(float delta); // consider setOrientation
    float getOrientation();
    glm::mat4 getTransMatrix(unsigned int frameTime);
    void updateNextPos();
    void turnLeft(bool turn);
    void turnRight(bool turn);
    void thrustForward(bool thrust);
    void setVisibility(bool visibility);
private:
    float const vertices[8] = { // update!!
        1.0f, 0.5f,  // Front of ship
        0.0f, 1.0f, // Rear right
        0.25f, 0.5f, // Rear centre
        0.0f, 0.0f // Rear left
    };
    float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation;
    float nextPosX, nextPosY, nextVelocityX, nextVelocityY;
    //glm::mat4 transformation;
    bool isVisible, isThrusting, isTurningLeft, isTurningRight;
    unsigned int timeSinceLastUpdate;
    float const drag = 5.0f/10000000.0f;
    float const thrust = 2.4f/10000000000.0f;
};

#endif