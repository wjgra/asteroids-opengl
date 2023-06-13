#ifndef _ASTEROIDS_GAME_OBJECT_HPP_
#define _ASTEROIDS_GAME_OBJECT_HPP_

#include "../include/drawable.hpp"

#include <glm/glm.hpp>

// Abstract class for simulated game objects to inherit from
class GameObject : public Drawable{
public:
    GameObject(const std::vector<float>& verts, const std::vector<GLuint>& elts) : Drawable(verts, elts) {};
    virtual void beginFrame(unsigned int frameTime) = 0;
    virtual void updateNextPos() = 0;
    virtual void updatePositions() = 0;
    virtual bool destroyThisFrame() = 0;
    virtual glm::mat4 getTransMatrix() = 0;
    int static const timeStep = 15000; // in microseconds
    int static timeSinceLastUpdate;
};

#endif