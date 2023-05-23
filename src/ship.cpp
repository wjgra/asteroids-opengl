#include "../include/ship.hpp"

static const float piValue = 3.1415926535897932385;

float Ship::vertices[]{
        1.0f, 0.5f,  // Front of ship
        0.0f, 1.0f, // Rear right
        0.25f, 0.5f, // Rear centre
        0.0f, 0.0f // Rear left
};

Ship::Ship(float s, float pX, float pY):scale(s), posX(pX), posY(pY){
    // random initial velocity
    velocityX = 150.0f/(1000000.0f);//0.0f;
    velocityY = -100.0f/(1000000.0f);//0.0f;
    orientation = 2.0f;//0.0f
    visible = false;
    isThrusting = false;
    timeSinceLastUpdate = 0;
}

Ship::~Ship(){
}

void Ship::show(){
    visible = true;
}

void Ship::hide(){
    visible = false;
}

bool Ship::isVisible(){
    return visible;
}

void Ship::changeOrientation(float delta){
    orientation += delta;
    // Keep in range [0, 2pi)
    while ( orientation >= 2 * piValue){
        orientation -= 2 *piValue;
    }
    while (orientation < 0){
        orientation += 2 * piValue;
    }
}

float Ship::getOrientation(){
    return orientation;
}

void Ship::updateNextPos(){
    //use vectors
    float timeStep = 15000; //timestep in microseconds
    float alpha = 1.0f/1000000.0f;

    // reuse quantities
    float expMinusAlphaT = exp(-alpha*timeStep);
    //std::cout << "ealphat: " << expMinusAlphaT <<"\n";
    nextPosX = posX + velocityX*(1-expMinusAlphaT)/alpha;
    nextPosY = posY + velocityY*(1-expMinusAlphaT)/alpha;

    nextVelocityX = velocityX * expMinusAlphaT;
    nextVelocityY = velocityY * expMinusAlphaT;
    std::cout << "old: "<< posX << ", " <<posY <<"\nnew: "<<nextPosX<<", "<<nextPosY<<"\n";
}

glm::mat4 Ship::getTransMatrix(unsigned int frameTime){
    // Init with zero call first
    float timeStep = 15000; //timestep in microseconds
    float alpha = 1.0f/1000000.0f;
    // non thrusting case
    
    timeSinceLastUpdate += frameTime;   
    while (timeSinceLastUpdate >= timeStep){
        posX = nextPosX;
        posY = nextPosY;
        velocityX = nextVelocityX;
        velocityY = nextVelocityY;
        updateNextPos();
        timeSinceLastUpdate -= timeStep;
    }
    
    float tInterp = float(timeSinceLastUpdate)/float(timeStep);    
    std::cout << "New matrix ("<<tInterp<<")\n";
    //std::cout << "Interp: "<< tInterp <<"\n";
    glm::vec3 shipPos = glm::vec3(posX*(1-tInterp)+nextPosX*tInterp,
        posY*(1-tInterp)+nextPosY*tInterp,
        0.0f);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, shipPos);
    trans = glm::translate(trans, glm::vec3(0.5*scale, 0.5*scale, 0.0));
    trans = glm::rotate(trans, orientation, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(-0.5*scale, -0.5*scale, 0.0));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));

    return trans;
};