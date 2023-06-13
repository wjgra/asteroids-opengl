#include "../include/game_state.hpp"

static float const piValue = 3.1415926535897932385;

GameState::GameState(unsigned int width, unsigned int height) : 
    winWidth{width},
    winHeight{height},
    shipScale{float(winWidth)/40.0f},
    wrapShader(".//shaders//vertex.vert", ".//shaders//fragment.frag")    
{
    GameObject::timeSinceLastUpdate = 0;
    ship = std::make_unique<Ship>(shipScale, winWidth/2.0f, winHeight/2.0f, 0.0f);

    // Create asteroids (temporary random selection)
    unsigned int const numAsteroids = 6;
    for (unsigned int i = 0; i < numAsteroids ; ++i){
        float temp = (float)i/(float)numAsteroids;
        std::unique_ptr<Asteroid> tempAst = std::make_unique<Asteroid>(shipScale*2.0f*(0.5f+temp), 
            winWidth*temp, 
            winHeight*temp, 
            2*piValue*temp, 
            8+(i%10));
        asteroids.push_back(std::move(tempAst));
    }

    // Activate shader to initialise uniforms
    wrapShader.useProgram();
        
    // Get locations of model/projection matrices
    uniformModelTrans = wrapShader.getUniformLocation("model");
    if (!uniformModelTrans)
        throw "Failed to get get location of uniform \'model\'";

    uniformProjTrans = wrapShader.getUniformLocation("projection");
    if (!uniformProjTrans)
        throw "Failed to get get location of uniform \'projection\'";

    // Set orthogonal projection matrix
    glm::mat4 projection = glm::ortho(0.0f, (float)winWidth, (float)winHeight, 0.0f, -1.0f, 1.0f); 
    glUniformMatrix4fv(uniformProjTrans, 1, GL_FALSE, glm::value_ptr(projection));

    // Define offsets for instance-based screen wrap effect
    glm::vec2 instanceOffsets[9] = {
        glm::vec2(0.0f, 0.0f), // centre
        glm::vec2((float)winWidth, 0.0f), // E
        glm::vec2((float)winWidth, (float)winHeight), // NE
        glm::vec2(0.0f, (float)winHeight), // N
        glm::vec2(-(float)winWidth, (float)winHeight), // NW
        glm::vec2(-(float)winWidth, 0.0f), // W
        glm::vec2(-(float)winWidth, -(float)winHeight), // SW
        glm::vec2(0.0f , -(float)winHeight), // S
        glm::vec2((float)winWidth, -(float)winHeight) //SE
    };

    // Copy offsets to uniform variables
    for (int i = 0; i < 9; ++i){
        GLint offsetLocation = wrapShader.getUniformLocation("offsets["+std::to_string(i)+"]");
        glUniform2fv(offsetLocation,1,glm::value_ptr(instanceOffsets[i]));
    }

    // Get location of colour uniform
    uniformColour = wrapShader.getUniformLocation("inputColour"); 

    

}

GameState::~GameState(){
}

void GameState::frame(unsigned int frameTime){
    // Prepare to render
    wrapShader.useProgram();

    glm::mat4 trans;

    GameObject::timeSinceLastUpdate += frameTime;
    ship->beginFrame(frameTime);
    for (auto&& ast : asteroids){
        ast->beginFrame(frameTime); // no-op currently
    }

    while(GameObject::timeSinceLastUpdate >= GameObject::timeStep){
        ship->updatePositions();
        ship->updateMissiles();
        for (auto&& ast : asteroids){
            ast->updatePositions();
        }
        // To do: modify missiles to conform with GameObject class
        // To do: check collisions
        GameObject::timeSinceLastUpdate -= GameObject::timeStep;
    }

    // Draw missiles
    glUniform4f(uniformColour, 0.3f, 0.4f, 1.0f, 1.0f);
    for (auto mis = ship->missiles.begin(); mis < ship->missiles.end(); /*no increment due to potential erasing*/){
        trans = (*mis)->getTransMatrix(frameTime);
        if ((*mis)->destroyThisFrame()){
            mis = ship->missiles.erase(mis);
        }
        else{
            glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
            (*mis)->draw();
            ++mis;
        }
    }

    // Draw asteroids
    glUniform4f(uniformColour, 0.8f, 0.8f, 0.7f, 1.0f);
    for (auto&& ast : asteroids)
    {
        trans = ast->getTransMatrix();
        glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
        ast->draw();
    }    

    // Draw ship (always on top)
    trans = ship->getTransMatrix();
    glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform4f(uniformColour, 0.5f, 0.6f, 1.0f, 1.0f);
    ship->draw();

}

void GameState::checkCollision(const Asteroid& ast, const Missile& mis){

}