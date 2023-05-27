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
public:
    float const vertices[8] = {
        0.5f, 0.0f,  // Front of ship
        -0.5f, 0.5f, // Rear right
        -0.25f, 0.0f, // Rear centre
        -0.5f, -0.5f // Rear left
    };
    GLuint const elements[5] = {0,1,2,3,0};
    float const drag = 5.0f/10000000.0f;
    float const thrust = 2.4f/10000000000.0f;
private:
    float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation;
    float nextPosX, nextPosY, nextVelocityX, nextVelocityY;
    //glm::mat4 transformation;
    bool isVisible, isThrusting, isTurningLeft, isTurningRight;
    unsigned int timeSinceLastUpdate;  
};

#endif