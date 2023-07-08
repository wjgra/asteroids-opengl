#ifndef _ASTEROIDS_GAME_OBJECT_HPP_
#define _ASTEROIDS_GAME_OBJECT_HPP_

#include "../include/drawable.hpp"

#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Abstract class for simulated game objects to inherit from
class GameObject : public Drawable{
public:
    friend class GameState;
    GameObject(const std::vector<float>& verts, const std::vector<GLuint>& elts, float s, float pX, float pY, float dir);
    virtual void beginFrame(unsigned int frameTime) = 0;
    virtual void updateNextPos() = 0;
    void updatePositions();
    bool toDestroyThisFrame() const;
    glm::mat4 getTransMatrix() const;
    virtual void destroy();
    int static const timeStep = 15000; // in microseconds
    int static timeSinceLastUpdate;
protected:
    float scale, posX, posY, velocityX, velocityY, orientation, nextOrientation, 
        nextPosX, nextPosY, nextVelocityX, nextVelocityY;
    bool toBeDestroyed;
};

#endif