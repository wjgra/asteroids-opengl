#ifndef _ASTEROID_HPP_
#define _ASTEROID_HPP_

#include "../include/glad/glad.h"
#include "../include/drawable.hpp"

#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Asteroid : public Drawable {
public:
    Asteroid(float s, float pX, float pY);
    ~Asteroid();
    // Simulation functions
    void updateNextPos();
    // -- Called every frame
    glm::mat4 getTransMatrix(unsigned int frameTime);
    // Utility functions
    void setVisibility(bool visibility);
    bool getVisibility() const;
public:
    // Simulation parameters
    float const speed = 1;
    float const rotPerTimeStep = 0.5e-2;
    float const timeStep = 15000; // in microseconds
private:
    float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation;
    float nextPosX, nextPosY;
    bool isVisible;
    unsigned int timeSinceLastUpdate; 
    //unsigned int size = 1;
    std::vector<float> const genVerts();
    std::vector<GLuint> const genElts();
};

#endif