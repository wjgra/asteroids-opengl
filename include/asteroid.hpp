#ifndef _ASTEROIDS_ASTEROID_HPP_
#define _ASTEROIDS_ASTEROID_HPP_

//#include "../include/glad/glad.h"
#include "../include/game_object.hpp"

class Asteroid : public GameObject {
public:
    Asteroid(float s, float pX, float pY, float dir, unsigned int sz, int rSpeed);
    ~Asteroid();
    // Simulation functions
    // -- Called every frame
    virtual void beginFrame(unsigned int frameTime) override;
    // -- Called every simulation timestep
    virtual void updateNextPos() override;
    // Utility functions
    void setVisibility(bool visibility);
    bool getVisibility() const;
public:
    // Simulation parameters
    unsigned int const size;
    unsigned const segments;
    float const speed = 1;
    float const rotPerTimeStep = 0.5e-2;
    float maxRadius; // temp!
private:
    std::vector<float> const genVerts(unsigned int sz);
    std::vector<GLuint> const genElts(unsigned int sz);
};

#endif