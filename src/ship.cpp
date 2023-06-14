#include "../include/ship.hpp"

static float const piValue = 3.1415926535897932385;

Ship::Ship(float s, float pX, float pY, float dir): 
    GameObject({
        0.5f, 0.0f,  // front of ship
        -0.5f, 0.5f, // rear right
        -0.25f, 0.0f, // rear centre
        -0.5f, -0.5f // rear left
    }, 
    {0,1,2,3,0}, s, pX, pY, dir){
    velocityX = 0.0f;
    velocityY = 0.0f;
    isVisible = false;
    isThrusting = false;
    timeSinceLastShot = minShootingInterval+1;
    isTurningLeft = false;
    isTurningRight = false;
    isShooting = false;
    updateNextPos();
}

Ship::~Ship(){
}

void Ship::beginFrame(unsigned int frameTime){
    timeSinceLastShot += frameTime;
}


// Updates the nextOrientation and nextPos/nextVelocity variables for the given timestep.
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
    missiles.push_back(std::move(newMissile));
}