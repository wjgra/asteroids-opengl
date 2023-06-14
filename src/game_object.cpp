#include "../include/game_object.hpp"

int GameObject::timeSinceLastUpdate = 0;

GameObject::GameObject(const std::vector<float>& verts, const std::vector<GLuint>& elts, float s, float pX, float pY, float dir) : 
    Drawable(verts, elts),
    scale{s}, posX{pX}, posY{pY}, orientation{dir}, toBeDestroyed{false}
{};


// Moves forward one timestep by copying nextPos->Pos etc. and checking for wrapping positions.
void GameObject::updatePositions(){
    posX = nextPosX;
    posY = nextPosY;
    velocityX = nextVelocityX;
    velocityY = nextVelocityY;
    orientation = nextOrientation;

    updateNextPos();

    // Wrap position when crossing edges of the screen
    if (posX > 640 && nextPosX > 640){
        posX -= 640;
        nextPosX -= 640;
    }
    else if (posX < 0 && nextPosX < 0){
        posX += 640;
        nextPosX += 640;
    }

    if (posY > 480 && nextPosY > 480){
        posY -= 480;
        nextPosY -= 480;
    }
    else if (posY < 0 && nextPosY < 0){
        posY += 480;
        nextPosY += 480;
    }

}

glm::mat4 GameObject::getTransMatrix(){
    // Linearly interpolate position and orientation
    float tInterp = float(GameObject::timeSinceLastUpdate)/float(GameObject::timeStep);
    
    glm::vec3 shipPos = glm::vec3(posX*(1-tInterp)+nextPosX*tInterp,
        posY*(1-tInterp)+nextPosY*tInterp,
        0.0f);

    float orient = (1-tInterp)*orientation+tInterp*nextOrientation;

    // Create transformation matrix
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, shipPos);
    trans = glm::rotate(trans, orient, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));

    return trans;
};