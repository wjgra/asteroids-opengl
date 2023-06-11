#include "../include/ship.hpp"

static float const piValue = 3.1415926535897932385;

Ship::Ship(float s, float pX, float pY): 
    Drawable({
        0.5f, 0.0f,  // front of ship
        -0.5f, 0.5f, // rear right
        -0.25f, 0.0f, // rear centre
        -0.5f, -0.5f // rear left
    }, 
    {0,1,2,3,0}/*,
    {0.5f, 0.6f, 1.0f, 1.0f}*/), 
    scale(s), posX(pX), posY(pY){
    velocityX = 0.0f;
    velocityY = 0.0f;
    orientation = 0.0f;
    isVisible = false;
    isThrusting = false;
    timeSinceLastUpdate = 0;
    timeSinceLastShot = minShootingInterval+1;
    isTurningLeft = false;
    isTurningRight = false;
    isShooting = false;
    updateNextPos();
}

Ship::~Ship(){
}

// Updates the orientation and nextPos/nextVelocity variables by stepping forwards one timeStep.
// Simulation is modelled as:
//      d^2x/dt^2 = thrust - drag * unit_vector(x), 
// where x and thrust are in R^2; drag > 0.
// It is assumed that the thrust force is of constant magnitude in the direction of the 
// ship's orientation at t = 0.
// When integrating over [0, t], this admits exact solution:
//      x = x_0 + (1/d)*(1-exp(-d*t))*v_0 + (1/d^2)*(d*t+exp(-d*t)-1)*T,
// where T = thrust, d = drag.
void Ship::updateNextPos(){
    float expMinusDragT = exp(-drag * timeStep);

    // Calculate pos/vel in absence of thrust
    nextPosX = posX + velocityX * (1 - expMinusDragT) / drag;
    nextPosY = posY + velocityY * (1 - expMinusDragT) / drag;

    nextVelocityX = velocityX * expMinusDragT;
    nextVelocityY = velocityY * expMinusDragT;

    // Correction term if thrusting 
    if (isThrusting){ 
        float Tx = thrust * cos(orientation);
        float Ty = thrust * sin(orientation);
        float temp = (drag * timeStep + expMinusDragT - 1) / (drag * drag);
        
        nextPosX += Tx * temp;
        nextPosY += Ty * temp;

        nextVelocityX += (1 - expMinusDragT) * Tx / drag;
        nextVelocityY += (1 - expMinusDragT) * Ty / drag;
    }

    // Update orientation based on inputs
    float rotationPerTimeStep = 2.0f / 100.0f;
    if (isTurningLeft == isTurningRight){
        nextOrientation = orientation;
    }
    else if (isTurningLeft){
        nextOrientation = orientation - 2.0f * piValue * rotationPerTimeStep;
    }
    else if (isTurningRight){
        nextOrientation = orientation + 2.0f * piValue * rotationPerTimeStep;
    }
}

// Returns a transformation matrix by interpolating between the current 
// and next position using the frame length.
glm::mat4 Ship::getTransMatrix(unsigned int frameTime){ // To do: consider separating update code from trans matric code
    // Update positions/velocities if timeStep passed since last update
    // int count = 0;
    timeSinceLastUpdate += frameTime;
    timeSinceLastShot += frameTime;
    while (timeSinceLastUpdate >= timeStep){
        posX = nextPosX;
        posY = nextPosY;
        velocityX = nextVelocityX;
        velocityY = nextVelocityY;
        orientation = nextOrientation;
        updateNextPos();
        updateMissiles();
        timeSinceLastUpdate -= timeStep;
        // count++;
    }
    
    /*// Alert if more than two updates performed per frame    
    if (count>2)
        std::cout << "New matrix ("<<count<<")\n";*/

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

    // **** possible split ****

    // Linearly interpolate position and orientation
    float tInterp = float(timeSinceLastUpdate)/float(timeStep);

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

void Ship::turnLeft(bool turn){
    isTurningLeft = turn;
}

void Ship::turnRight(bool turn){
    isTurningRight = turn;
}

void Ship::thrustForward(bool thrust){
    isThrusting = thrust;
}

// Sets the orientation of the ship manually.
void Ship::setOrientation(float orient){
    orientation = orient;
}

// Returns the current orientation of the ship.
float Ship::getOrientation() const{
    return orientation;
}

// Sets the visibility of the ship (currently unused).
void Ship::setVisibility(bool visibility){
    isVisible = visibility;
}

// Returns the current visibility of the ship (currently unused).
bool Ship::getVisibility() const{
    return isVisible;
}

// Sets the current shooting state of the ship.
void Ship::shootMissile( bool shooting){
    isShooting = shooting;
}

// Spawns a missile at current timestep if shooting and sufficient time passed.
void Ship::updateMissiles(){
    if (isShooting && (timeSinceLastShot >= minShootingInterval)){
        spawnMissile();
        timeSinceLastShot = 0;
    }
    else{
        timeSinceLastShot += timeStep;
    }
}


// Creates a missile at posX, posY at the beginning of the timeStep
void Ship::spawnMissile(){
    std::unique_ptr<Missile> newMissile = std::make_unique<Missile>(scale, posX, posY, orientation, velocityX, velocityY);
    //newMissile->setUpBuffers();
    missiles.push_back(std::move(newMissile));
    timeSinceLastShot = 0;
}