#include "../include/asteroid.hpp"

static float const piValue = 3.1415926535897932385;

Asteroid::Asteroid(float s, float pX, float pY, float dir, unsigned int sz, int rSpeed): 
    GameObject(genVerts(sz), genElts(sz), s, pX, pY, dir),
    size{sz},
    segments{7+3*sz},
    speed{(5-sz)/2},
    rotPerTimeStep{0.005*rSpeed}
{
    maxRadius = 1.25*s*(1u << sz)/2.0f;
    velocityX = speed*cos(dir);
    velocityY = speed*sin(dir);
    updateNextPos();
}

Asteroid::~Asteroid(){
}

void Asteroid::beginFrame(unsigned int frameTime){
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

std::vector<float> const Asteroid::genVerts(unsigned int sz){
    unsigned int segs = 7+3*sz;
    // Set up mesh data
    std::vector<float> temp(2*segs);
    float const unitAngle = 2*piValue/(float)segs;

    float tempX((1u << sz)/2.0f), tempY = 0.0f;
    float ang = 0;
    for (unsigned int i = 0; i<segs; ++i){
        float tempRand = (rand()%11)/10.0f;
        float radiusScale = 0.85f + 0.3*tempRand*tempRand*tempRand;
        temp[2*i] = radiusScale*(tempX*cos(ang)-tempY*sin(ang));
        temp[2*i+1] = radiusScale*(tempX*sin(ang)+tempY*cos(ang));
        ang += unitAngle;
    }
    return temp;
}

std::vector<GLuint> const Asteroid::genElts(unsigned int sz){
    unsigned int segs = 7+3*sz;
    std::vector<GLuint> temp2(segs+1);
    for (unsigned int i = 0; i<segs; ++i){
        temp2[i] = i;
    }
    temp2[segs] = 0;
    return temp2;
}