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
    void show();
    void hide();
    bool isVisible();
    void changeOrientation(float delta); // consider setOrientation
    float getOrientation();
    glm::mat4 getTransMatrix(unsigned int frameTime);
    void updateNextPos();
    void turnLeft(bool);
    void turnRight(bool);
    void thrustForward(bool);
private:
    static float vertices[];
    float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation;
    float nextPosX, nextPosY, nextVelocityX, nextVelocityY;
    //glm::mat4 transformation;
    bool visible, isThrusting, isTurningLeft, isTurningRight;
    unsigned int timeSinceLastUpdate;

};

#endif