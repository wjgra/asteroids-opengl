#include "../include/ship.hpp"

static const float piValue = 3.1415926535897932385;

Ship::Ship(float s, float pX, float pY):scale(s), posX(pX), posY(pY){
    velocityX = 0.0f;//150.0f/(1000000.0f);//0.0f;// random initial velocity
    velocityY = 0.0f;//-100.0f/(1000000.0f);//0.0f;
    orientation = 0.0f;//2.0f;//0.0f
    isVisible = false;
    isThrusting = false;
    timeSinceLastUpdate = 0;
    isTurningLeft = false;
    isTurningRight = false;
}

Ship::~Ship(){
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
    

    // reuse quantities
    float expMinusDragT = exp(-drag*timeStep);
    //std::cout << "expMinusDragt: " << expMinusDragT <<"\n";
    nextPosX = posX + velocityX*(1-expMinusDragT)/drag;
    nextPosY = posY + velocityY*(1-expMinusDragT)/drag;
    nextVelocityX = velocityX * expMinusDragT;
    nextVelocityY = velocityY * expMinusDragT;

    //float T = drag*480.0f/(2000000.0f);
    float Tx = thrust*cos(orientation);
    float Ty = thrust*sin(orientation);

    float temp = (drag*timeStep+expMinusDragT-1)/(drag*drag);
    if (isThrusting){ //correction term if thrusting (assume thrust in direction at beginning of interval)
        
        
        nextPosX += Tx*temp;
        nextPosY += Ty*temp;

        nextVelocityX += (1-expMinusDragT)*Tx/drag;
        nextVelocityY += (1-expMinusDragT)*Ty/drag;
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
    //float drag = 1.0f/1000000.0f;
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

void Ship::setVisibility(bool visibility){
    isVisible = visibility;
}