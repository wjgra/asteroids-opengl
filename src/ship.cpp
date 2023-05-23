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
    velocityX = 0.0f;//150.0f/(1000000.0f);//0.0f;
    velocityY = 0.0f;//-100.0f/(1000000.0f);//0.0f;
    orientation = 0.0f;//2.0f;//0.0f
    visible = false;
    isThrusting = false;
    timeSinceLastUpdate = 0;
    isTurningLeft = false;
    isTurningRight = false;
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

    float T = alpha*480.0f/(2000000.0f);
    float Tx = T*cos(orientation);
    float Ty = T*sin(orientation);

    float temp = (alpha*timeStep+expMinusAlphaT-1)/(alpha*alpha);
    if (isThrusting){ //correction term if thrusting (assume thrust in direction at beginning of interval)
        
        
        nextPosX += Tx*temp;
        nextPosY += Ty*temp;

        nextVelocityX += (1-expMinusAlphaT)*Tx/alpha;
        nextVelocityY += (1-expMinusAlphaT)*Ty/alpha;
    }

    //std::cout << "old: "<< posX << ", " <<posY <<"\nnew: "<<nextPosX<<", "<<nextPosY<<"\n";
    float factor = 2.0f/100.0f;
    //std::cout << 2.0f*piValue*factor <<"\n";
    if (isTurningLeft && isTurningRight){ // use xor
        nextOrientation = orientation;
    }
    else if (isTurningLeft){
        nextOrientation = orientation-2.0f*piValue*factor;
    }
    else if (isTurningRight){
        nextOrientation = orientation+2.0f*piValue*factor;
    }
    else{
        nextOrientation = orientation;
    }
    //isTurningLeft = false;
    //isTurningRight = false;//temp removal
    
}

glm::mat4 Ship::getTransMatrix(unsigned int frameTime){
    // Init with zero call first
    float timeStep = 15000; //timestep in microseconds
    float alpha = 1.0f/1000000.0f;
    // non thrusting case
    
    timeSinceLastUpdate += frameTime;   
    int count = 0;
    while (timeSinceLastUpdate >= timeStep){
        posX = nextPosX;
        posY = nextPosY;
        velocityX = nextVelocityX;
        velocityY = nextVelocityY;
        orientation = nextOrientation;

        updateNextPos();
        timeSinceLastUpdate -= timeStep;
        count++;
    }
    
    // temporary wrapping - need to create second instance to ensure smooth transition
    if (posX > 640 && nextPosX > 640){
        posX-=640;
        nextPosX-=640;
    }
    else if (posX <0 && nextPosX <0){
        posX+=640;
        nextPosX+=640;
    }

    if (posY > 480 && nextPosY > 480){
        posY-=480;
        nextPosY-=480;
    }
    else if (posY <0 && nextPosY <0){
        posY+=480;
        nextPosY+=480;
    }


    float tInterp = float(timeSinceLastUpdate)/float(timeStep);    
    if (count>2)
        std::cout << "New matrix ("<<count<<")\n";
    
    //std::cout << "Interp: "<< tInterp <<"\n";
    glm::vec3 shipPos = glm::vec3(posX*(1-tInterp)+nextPosX*tInterp,
        posY*(1-tInterp)+nextPosY*tInterp,
        0.0f);

    float orient = (1-tInterp)*orientation+tInterp*nextOrientation;

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, shipPos);
    trans = glm::translate(trans, glm::vec3(0.5*scale, 0.5*scale, 0.0));
    trans = glm::rotate(trans, orient, glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(-0.5*scale, -0.5*scale, 0.0));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));

    return trans;
};

void Ship::turnLeft(bool turn){
    isTurningLeft = turn;
}

void Ship::turnRight(bool turn){
    isTurningRight = turn;
}

void Ship::thrustForward(bool thrust){
    isThrusting = thrust;
}