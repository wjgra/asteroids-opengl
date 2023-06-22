#include "../include/game_state.hpp"

static float const piValue = 3.1415926535897932385;

GameState::GameState(unsigned int width, unsigned int height) : 
    winWidth{width},
    winHeight{height},
    shipScale{float(winWidth)/40.0f},
    wrapShader(".//shaders//vertex.vert", ".//shaders//fragment.frag")    
{
    screen = Screen::menu;
    newGame({10,5,6,1});

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
    switch (screen){
        case GameState::Screen::play:
            framePlay(frameTime);
        break;
        case GameState::Screen::pause:
            framePause(frameTime);
        break;
        case GameState::Screen::menu:
            frameMenu(frameTime);
        break;
        case GameState::Screen::score:
            frameScore(frameTime);
        break;
        default:
        break;
    }
}

bool GameState::checkCollisionCoarse(const Asteroid& ast, const Missile& mis){
    return (std::abs(mis.posX-ast.posX) <= ast.maxRadius) && (std::abs(mis.posY-ast.posY) <= ast.maxRadius);
}

void GameState::handleEventsPlay(SDL_Event const& event){
    switch(event.type){
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_ESCAPE:
                    // Go to pause screen and clear all input
                    ship->turnLeft(false);
                    ship->turnRight(false);
                    ship->thrustForward(false);
                    ship->shootMissile(false);
                    screen = Screen::pause; // to do: implement state change function?
                    break;
                case SDL_SCANCODE_LEFT:
                    ship->turnLeft(true);
                    break;
                case SDL_SCANCODE_RIGHT:
                    ship->turnRight(true);
                    break;
                case SDL_SCANCODE_UP:
                    ship->thrustForward(true);
                    break;
                case SDL_SCANCODE_DOWN:
                    ship->shootMissile(true);
                    break;
                case SDL_SCANCODE_RETURN: // temp - go to score screen
                    screen = Screen::score;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_LEFT:
                    ship->turnLeft(false);
                    break;
                case SDL_SCANCODE_RIGHT:
                    ship->turnRight(false);
                    break;
                case SDL_SCANCODE_UP:
                    ship->thrustForward(false);
                    break;
                case SDL_SCANCODE_DOWN:
                    ship->shootMissile(false);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void GameState::handleEventsPause(SDL_Event const& event){
    switch(event.type){
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_TAB:
                    screen = Screen::menu;
                    break;
                case SDL_SCANCODE_RETURN:
                    screen = Screen::play;
                    break;
                default:
                    break;
            }
    }
}

void GameState::handleEventsMenu(SDL_Event const& event){
    switch(event.type){
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_RETURN:
                    newGame({2,5,6,1});
                    screen = Screen::play;
                    break;
                case SDL_SCANCODE_TAB:
                    screen = Screen::quit;
                    break;
                default:
                    break;
            }
    }
}

void GameState::handleEventsScore(SDL_Event const& event){
        switch(event.type){
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_RETURN:
                    screen = Screen::menu;
                    break;
                default:
                    break;
            }
    }
}

void GameState::framePlay(unsigned int frameTime){
    // Prepare to render
    wrapShader.useProgram();


    GameObject::timeSinceLastUpdate += frameTime;
    ship->beginFrame(frameTime);
    /*for (auto&& ast : asteroids){
        ast->beginFrame(frameTime); // no-op currently
    }*/
    for (auto&& mis : ship->missiles){
        mis->beginFrame(frameTime);
    }

    while(GameObject::timeSinceLastUpdate >= GameObject::timeStep){
        ship->updatePositions();
        ship->updateMissiles();
        for (auto&& ast : asteroids){
            ast->updatePositions();
        }
        for (auto&& mis : ship->missiles){
            mis->updatePositions();
        }
        // Check collisions
        for (auto&& mis : ship->missiles){
            for (auto&& ast : asteroids){
                if (!(mis->toDestroyThisFrame()) && !ast->toDestroyThisFrame() && checkCollisionCoarse(*ast,*mis)){
                    // To do: fine collision detection
                    score += ast->size;
                    mis->destroy();
                    ast->destroy();
                    if (ast->size != 0)
                    {
                        float deflection = piValue/10.0f;
                        float dirAst = atan2(ast->velocityY, ast->velocityX);
                        int rotSpeedModifier = (rand() % 5) - 2;
                        if (!rotSpeedModifier) ++rotSpeedModifier;
                        std::unique_ptr<Asteroid> newAst1 = std::make_unique<Asteroid>(shipScale,ast->posX,ast->posY,dirAst+deflection,ast->size-1, rotSpeedModifier);
                        std::unique_ptr<Asteroid> newAst2 = std::make_unique<Asteroid>(shipScale,ast->posX,ast->posY,dirAst-deflection,ast->size-1, rotSpeedModifier);
                        asteroids.push_back(std::move(newAst1));
                        asteroids.push_back(std::move(newAst2));
                    }
                    break;
                }
            }
        }
        GameObject::timeSinceLastUpdate -= GameObject::timeStep;
    }
    drawMissiles();
    drawAsteroids();
    drawShip();
    textRen.drawString("SCORE: " + std::to_string(score), 16.0f, winWidth/20, winHeight/20);
}

void GameState::framePause(unsigned int frameTime){
    // Prepare to render
    wrapShader.useProgram();

    drawMissiles();
    drawAsteroids();
    drawShip();

    textRen.drawString("SCORE: " + std::to_string(score), 16.0f, winWidth/20, winHeight/20);

    // Draw pause menu
    textRen.drawStringCentred("PAUSED", 32.0f, winWidth/2, winHeight/2-32.0f);
    textRen.drawStringCentred("[ENTER] RETURN TO GAME", 16.0f, winWidth/2, winHeight/2+32.0f);
    textRen.drawStringCentred("[TAB] EXIT TO MENU", 16.0f, winWidth/2, winHeight/2+64.0f);
}

void GameState::frameMenu(unsigned int frameTime){

    GameObject::timeSinceLastUpdate += frameTime;
    while(GameObject::timeSinceLastUpdate >= GameObject::timeStep){
        for (auto&& ast : asteroids){
            ast->updatePositions();
        }
        GameObject::timeSinceLastUpdate -= GameObject::timeStep;
    }
    

    wrapShader.useProgram();
    drawAsteroids();

    textRen.drawStringCentred("A HOMAGE TO ATARI'S", 16.0f, winWidth/2, winHeight/2-64.0f);
    textRen.drawStringCentred("ASTEROIDS", 64.0f, winWidth/2, winHeight/2-32.0f);
    textRen.drawStringCentred("[ENTER] PLAY GAME", 16.0f, winWidth/2, winHeight/2+64.0f);
    textRen.drawStringCentred("[TAB] QUIT", 16.0f, winWidth/2, winHeight/2+96.0f); // Do not display in web version
}

void GameState::frameScore(unsigned int frameTime){
    
    GameObject::timeSinceLastUpdate += frameTime;
    while(GameObject::timeSinceLastUpdate >= GameObject::timeStep){
        for (auto&& ast : asteroids){
            ast->updatePositions();
        }
        GameObject::timeSinceLastUpdate -= GameObject::timeStep;
    }
    

    wrapShader.useProgram();
    //drawMissiles();
    drawAsteroids(); // to do: do not change to score screen until all missiles expired
    //drawShip();

    textRen.drawStringCentred("SCORE: " + std::to_string(score), 32.0f, winWidth/2, winHeight/2-32.0f);
    textRen.drawStringCentred("[ENTER] EXIT TO MENU", 16.0f, winWidth/2, winHeight/2+32.0f);
}

void GameState::drawShip(){     // To do: if not destroyed...
    glm::mat4 trans = ship->getTransMatrix();
    glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform4f(uniformColour, 0.5f, 0.6f, 1.0f, 1.0f);
    ship->draw();
}

void GameState::drawMissiles(){
    glm::mat4 trans;
    glUniform4f(uniformColour, 0.3f, 0.4f, 1.0f, 1.0f);
    for (auto&& it = ship->missiles.begin(); it < ship->missiles.end(); /*no increment due to potential erasing*/){
       if ((*it)->toDestroyThisFrame()){
            it = ship->missiles.erase(it);
        }
        else{
            trans = (*it)->getTransMatrix();
            glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
            (*it)->draw();
            ++it;
        }
    }
}
    
void GameState::drawAsteroids(){
    glm::mat4 trans;
    glUniform4f(uniformColour, 0.8f, 0.8f, 0.7f, 1.0f);
    for (auto&& it = asteroids.begin(); it < asteroids.end(); /*no increment due to potential erasing*/){
       if ((*it)->toDestroyThisFrame()){
            it = asteroids.erase(it);
        }
        else{
            trans = (*it)->getTransMatrix();
            glUniformMatrix4fv(uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
            (*it)->draw();
            ++it;
        }
    }
}

void GameState::newGame(std::vector<unsigned int> astLayout){
    score = 0;
    GameObject::timeSinceLastUpdate = 0;
    ship = std::make_unique<Ship>(shipScale, winWidth/2.0f, winHeight/2.0f, 0.0f);

    asteroids.clear();
    srand(time(NULL));
    
    for (unsigned int size = 0 ; size < std::min((int)astLayout.size(), 4); ++size){
        for (unsigned int i = 0 ; i < astLayout[size]; ++i){
            float x = winWidth * (0.66f + (rand() % 20) / 30.0f);
            float y = winHeight * (0.66f + (rand() % 20) / 30.0f);
            float dir = 2 * piValue * (rand() % 60) / 60.0f;
            int rotSpeedModifier = (rand() % 5) - 2;
            if (!rotSpeedModifier) ++rotSpeedModifier;
            std::unique_ptr<Asteroid> tempAst = std::make_unique<Asteroid>(shipScale, x, y, dir, size, rotSpeedModifier);
            asteroids.push_back(std::move(tempAst));
        }
    }
}