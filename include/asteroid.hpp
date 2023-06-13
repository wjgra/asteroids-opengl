#ifndef _ASTEROIDS_ASTEROID_HPP_
#define _ASTEROIDS_ASTEROID_HPP_

#include "../include/glad/glad.h"
#include "../include/game_object.hpp"

#include <chrono>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Asteroid : public GameObject {
public:
    Asteroid(float s, float pX, float pY, float dir, unsigned int segs);
    ~Asteroid();
    // Simulation functions
    // -- Called every frame
    virtual void beginFrame(unsigned int frameTime) override;
    virtual bool destroyThisFrame() override;
    // -- Called every simulation timestep
    virtual void updateNextPos() override;
    // Utility functions
    void setVisibility(bool visibility);
    bool getVisibility() const;
public:
    // Simulation parameters
    float const speed = 1;
    float const rotPerTimeStep = 0.5e-2;
    //unsigned const segments;
    float maxRadius; // temp!
private:
    //unsigned int size = 1;
    std::vector<float> const genVerts(unsigned int segments);
    std::vector<GLuint> const genElts(unsigned int segments);
};

#endif