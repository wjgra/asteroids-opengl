#include "../include/asteroid.hpp"

static float const piValue = 3.1415926535897932385;

Asteroid::Asteroid(float s, float pX, float pY, float dir, unsigned int segs): 
    GameObject(genVerts(segs), genElts(segs), s, pX, pY, dir){
    velocityX = speed*cos(dir);
    velocityY = speed*sin(dir);
    maxRadius = 1.25*s; //temp!
    updateNextPos();
}

Asteroid::~Asteroid(){
}

void Asteroid::beginFrame(unsigned int frameTime){
}

bool Asteroid::destroyThisFrame(){
    return false;
}

// Updates the orientation and nextPos/nextVelocity variables by stepping forwards one timeStep.
void Asteroid::updateNextPos(){
    // Calculate pos
    nextPosX = posX + velocityX;
    nextPosY = posY + velocityY;

    nextVelocityX = velocityX;
    nextVelocityY = velocityY;

    // Update orientation 
    nextOrientation = orientation - 2.0f * piValue * rotPerTimeStep;
    
}

std::vector<float> const Asteroid::genVerts(unsigned int segments){
    // Set up mesh data
    std::vector<float> temp(2*segments);
    float const unitAngle = 2*piValue/segments;
    
    /*float const tempX = 1.0f, tempY = 0.0f;
    for (unsigned int i = 0; i<segments; ++i){
        float radiusScale = 0.7f + 0.6f*(rand()%11)/10.0f;

        float rotAngle = unitAngle*i;
        temp[2*i] = radiusScale*(tempX*cos(rotAngle)-tempY*sin(rotAngle));
        temp[2*i+1] = radiusScale*(tempX*sin(rotAngle)+tempY*cos(rotAngle));
    }
    
    return temp;*/

    float tempX = 1.0f, tempY = 0.0f;
    for (unsigned int i = 0; i<segments; ++i){
        temp[2*i] = tempX;
        temp[2*i+1] = tempY;
        float tempVal = tempX*cos(unitAngle)-tempY*sin(unitAngle);

        float radiusScale = 0.75f + 0.5f*(rand()%11)/10.0f;

        tempY = radiusScale*tempX*sin(unitAngle)+tempY*cos(unitAngle);
        tempX = radiusScale*tempVal;
    }
    return temp;
}

std::vector<GLuint> const Asteroid::genElts(unsigned int segments){
    std::vector<GLuint> temp2(segments+1);
    for (unsigned int i = 0; i<segments; ++i){
        temp2[i] = i;
    }
    temp2[segments] = 0;
    return temp2;
}