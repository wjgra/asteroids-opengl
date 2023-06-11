#include "../include/asteroid.hpp"

static float const piValue = 3.1415926535897932385;

Asteroid::Asteroid(float s, float pX, float pY, float dir, unsigned int segs): 
    Drawable(genVerts(segs), genElts(segs)),
    scale{s}, posX{pX}, posY{pY}, orientation{dir}
{
    velocityX = speed*cos(dir);
    velocityY = speed*sin(dir);
    isVisible = false;
    timeSinceLastUpdate = 0;
    maxRadius = 1.25*s; //temp!
    updateNextPos();
}

Asteroid::~Asteroid(){
}

// Updates the orientation and nextPos/nextVelocity variables by stepping forwards one timeStep.
void Asteroid::updateNextPos(){
    // Calculate pos
    nextPosX = posX + velocityX;
    nextPosY = posY + velocityY;

    // Update orientation 
    nextOrientation = orientation - 2.0f * piValue * rotPerTimeStep;
    
}

// Returns a transformation matrix by interpolating between the current 
// and next position using the frame length.
glm::mat4 Asteroid::getTransMatrix(unsigned int frameTime){
    // Update positions/velocities if timeStep passed since last update
    timeSinceLastUpdate += frameTime;   
    //int count = 0;
    while (timeSinceLastUpdate >= timeStep){
        posX = nextPosX;
        posY = nextPosY;
        orientation = nextOrientation;
        updateNextPos();
        timeSinceLastUpdate -= timeStep;
        //count++;
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


// Sets the visibility of the asteroid (currently unused).
void Asteroid::setVisibility(bool visibility){
    isVisible = visibility;
}

// Returns the current visibility of the asteroid (currently unused).
bool Asteroid::getVisibility() const{
    return isVisible;
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