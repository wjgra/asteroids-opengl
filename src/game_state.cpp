#include "../include/game_state.hpp"

static float const piValue = 3.1415926535897932385;

GameState::GameState(unsigned int width, unsigned int height) : 
    winWidth{width},
    winHeight{height},
    shipScale{float(winWidth)/40.0f},
    ship(shipScale, winWidth/2.0f, winHeight/2.0f),
    wrapShader(".//shaders//vertex.vert", ".//shaders//fragment.frag")    
{
    // Create asteroids
    unsigned int const numAsteroids = 4;
    for (unsigned int i = 0; i < numAsteroids ; ++i){
        std::cout << "Creating asteroid " << i <<" (" <<8+(i%10)<<")\n";
        float temp = (float)i/(float)numAsteroids;
        asteroids.emplace_back(shipScale*2.0f*(0.5f+temp), 
            winWidth*temp, 
            winHeight*temp, 
            2*piValue*temp, 
            8+(i%10));
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