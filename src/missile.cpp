#include "../include/missile.hpp"

static float const piValue = 3.1415926535897932385;

Missile::Missile(float s, float pX, float pY, float dir, float shipVelX, float shipVelY): 
    Drawable({
        0.25f, 0.0f,  // front of missile
        0.0f, 0.25f,
        -0.25f, 0.0f,
        0.0f, -0.25f // back of missile
    }, 
    {0, 1, 3, 2}), 
    scale(s), posX(pX), posY(pY), orientation{dir}{
    velocityX = shipVelX + missileSpeed*cos(dir);
    velocityY = shipVelY + missileSpeed*sin(dir);
    setDrawingMode(GL_TRIANGLE_STRIP);
    timeSinceLastUpdate = 0;
    timeUntilDestroyed = lifetime;
    toBeDestroyed = false;
    updateNextPos();
}

Missile::~Missile(){
    releaseBuffers();
}

void Missile::updateNextPos(){
    nextPosX = posX + timeStep * velocityX;
    nextPosY = posY + timeStep * velocityY;
}

glm::mat4 Missile::getTransMatrix(unsigned int frameTime){
    // Update positions/velocities if timeStep passed since last update
    timeSinceLastUpdate += frameTime;
    if (frameTime >= timeUntilDestroyed){
        toBeDestroyed = true;
    }
    else{
        timeUntilDestroyed -= frameTime;
    }
    
    //int count = 0;
    while (timeSinceLastUpdate >= timeStep){
        posX = nextPosX;
        posY = nextPosY;
        updateNextPos();
        timeSinceLastUpdate -= timeStep;
        //count++;
    }
    
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

    // Linearly interpolate position and orientation
    float tInterp = float(timeSinceLastUpdate)/float(timeStep);

    glm::vec3 shipPos = glm::vec3(posX*(1-tInterp)+nextPosX*tInterp,
        posY*(1-tInterp)+nextPosY*tInterp,
        0.0f);

    // Create transformation matrix
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, shipPos);
    trans = glm::rotate(trans, orientation, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));

    return trans;
}

bool Missile::destroyThisFrame(){
    return toBeDestroyed;
}