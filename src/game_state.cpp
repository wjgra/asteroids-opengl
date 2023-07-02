#include "../include/game_state.hpp"

static float const piValue = 3.1415926535897932385;

GameState::GameState(unsigned int width, unsigned int height) : 
    winWidth{width},
    winHeight{height},
    shipScale{float(winWidth)/40.0f},
    wrapShader(".//shaders//wrap.vert", ".//shaders//wrap.frag")    
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

void GameState::handleEvents(SDL_Event const& event){
    switch (screen){
        case GameState::Screen::play:
            handleEventsPlay(event);
        break;
        case GameState::Screen::pause:
            handleEventsPause(event);
        break;
        case GameState::Screen::menu:
            handleEventsMenu(event);
        break;
        case GameState::Screen::score:
            handleEventsScore(event);
        break;
        default:
        break;
    }
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
                // case SDL_SCANCODE_RETURN: // temp - go to score screen
                   // screen = Screen::score;
                    // break;
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
                    newGame();
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
   
        // Check missile collisions
        for (auto&& mis : ship->missiles){
            for (auto&& ast : asteroids){
                if (!(mis->toDestroyThisFrame()) && !(ast->toDestroyThisFrame()) 
                    && checkCollisionCoarse(*ast,*mis) && checkCollisionFine(*ast, *mis)){
                    // To do: fine collision detection
                    score += (4 - ast->size);
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

        // Check ship collisions
        for (auto&& ast : asteroids){
            if (!(ast->toDestroyThisFrame()) && checkCollisionCoarse(*ast, *ship) && checkCollisionFine(*ast, *ship)){
                // Ship and asteroid are destroyed in collision
                ast->destroy();
                score += (4 - ast->size);
                // Spawn new asteroids (duplicate)
                                    if (ast->size != 0)
                    {
                        float deflection = piValue/10.0f;
                        float dirAst = atan2(ship->velocityY, ship->velocityX); // modified! based on ship speed
                        int rotSpeedModifier = (rand() % 5) - 2;
                        if (!rotSpeedModifier) ++rotSpeedModifier;
                        std::unique_ptr<Asteroid> newAst1 = std::make_unique<Asteroid>(shipScale,ast->posX,ast->posY,dirAst+deflection,ast->size-1, rotSpeedModifier);
                        std::unique_ptr<Asteroid> newAst2 = std::make_unique<Asteroid>(shipScale,ast->posX,ast->posY,dirAst-deflection,ast->size-1, rotSpeedModifier);
                        asteroids.push_back(std::move(newAst1));
                        asteroids.push_back(std::move(newAst2));
                    }
                screen = Screen::score;
            }
        }

        if (asteroids.size() == 0){
            newWave();
        }

        GameObject::timeSinceLastUpdate -= GameObject::timeStep;
    }

    drawMissiles();
    drawAsteroids();
    drawShip();
    textRen.drawString("SCORE: " + std::to_string(score), 16.0f, winWidth/20, winHeight/20);
    textRen.drawString("WAVE: " + std::to_string(wave), 16.0f, 15 * winWidth/20, winHeight/20);

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

bool GameState::checkCollisionCoarse(const Asteroid& ast, const Missile& mis) const{
    float colRadius = ast.maxRadius + 0.25f * shipScale;
    return (std::abs(mis.posX-ast.posX) <= colRadius) && (std::abs(mis.posY-ast.posY) <= colRadius);
}

bool GameState::checkCollisionFine(const Asteroid& ast, const Missile& mis) const{
    // Whilst I could make this 'correct' like for ship-asteroid collisions (using trianglesIntersect function),
    // the game is already hard enough! Generous hitboxes are the least I can do...
    return true;
}

// Ship argument not needed, but allows for extension to multiplayer
bool GameState::checkCollisionCoarse(const Asteroid& ast, const Ship& sh) const{
    float colRadius = ast.maxRadius + 0.707f*shipScale;
    return (std::abs(sh.posX-ast.posX) <= colRadius) && (std::abs(sh.posY-ast.posY) <= colRadius);
}

bool GameState::checkCollisionFine(const Asteroid& ast, const Ship& sh) const{
    Triangle shipL, shipR, astSeg;
    auto xRot =  [](float x, float y, float angle) {return std::cos(angle) * x - std::sin(angle) * y;};
    auto yRot =  [](float x, float y, float angle) {return std::sin(angle) * x + std::cos(angle) * y;};
    /*
    for (int i = 0; i < 3; ++i){
        // float alpha = sh.orientation;
        // shipR.points[2*i] = shipScale*(std::cos(alpha)*sh.vertices[2*i]-std::sin(alpha)*sh.vertices[2*i+1]) + sh.posX - ast.posX;
        // shipR.points[2*i+1] = shipScale*(std::sin(alpha)*sh.vertices[2*i]+std::cos(alpha)*sh.vertices[2*i+1]) + sh.posY - ast.posY;
        float tempX = shipScale * xRot(sh.vertices[2*i], sh.vertices[2*i+1], sh.orientation) + sh.posX - ast.posX;
        float tempY = shipScale * yRot(sh.vertices[2*i], sh.vertices[2*i+1], sh.orientation) + sh.posY - ast.posY;
        shipR.points[2*i] = xRot(tempX, tempY, -ast.orientation);
        shipR.points[2*i + 1] = yRot(tempX, tempY, -ast.orientation);
    }

    for (int i = 3; i < 6; ++i){
        float x = i != 5 ? sh.vertices[2*i] : sh.vertices[0];
        float y = i != 5 ? sh.vertices[2*i+1] : sh.vertices[1];
        float tempX = shipScale * xRot(x, y, sh.orientation) + sh.posX - ast.posX;
        float tempY = shipScale * yRot(x, y, sh.orientation) + sh.posY - ast.posY;
        shipL.points[2*i] = xRot(tempX, tempY, -ast.orientation);
        shipL.points[2*i + 1] = yRot(tempX, tempY, -ast.orientation);
    }
    

    float lastX = ast.vertices.end()[-2]*shipScale, lastY = ast.vertices.end()[-1]*shipScale;
    float currentX, currentY;   

    for (int i = 0 ; i < ast.vertices.size()/2; ++i){
        currentX = ast.vertices[2*i]*shipScale;
        currentY = ast.vertices[2*i+1]*shipScale;
        astSeg = {0.0f, 0.0f, lastX, lastY, currentX, currentY};
        lastX = currentX, lastY = currentY;
        if (trianglesIntersect(shipL, astSeg) || trianglesIntersect(shipR, astSeg)) 
            return true;
    }
    */
     for (int i = 0; i < 3; ++i){
 shipR.points[2*i] = shipScale * xRot(sh.vertices[2*i], sh.vertices[2*i+1], sh.orientation) + sh.posX;
        shipR.points[2*i + 1] = shipScale * yRot(sh.vertices[2*i], sh.vertices[2*i+1], sh.orientation) + sh.posY;

    }

    for (int i = 3; i < 6; ++i){
        float x = i != 5 ? sh.vertices[2*i] : sh.vertices[0];
        float y = i != 5 ? sh.vertices[2*i+1] : sh.vertices[1];
        shipL.points[2*(i-3)] = shipScale * xRot(x, y, sh.orientation) + sh.posX;
        shipL.points[2*(i-3) + 1] = shipScale * yRot(x, y, sh.orientation) + sh.posY;
    }
    
    float lastX = shipScale * xRot(ast.vertices.end()[-2], ast.vertices.end()[-1], ast.orientation) + ast.posX; 
    float lastY = shipScale * yRot(ast.vertices.end()[-2], ast.vertices.end()[-1], ast.orientation) + ast.posY;
    float currentX, currentY;   

    for (int i = 0 ; i < ast.vertices.size()/2; ++i){
        currentX = shipScale * xRot(ast.vertices[2*i], ast.vertices[2*i+1], ast.orientation) + ast.posX; 
        currentY = shipScale * yRot(ast.vertices[2*i], ast.vertices[2*i+1], ast.orientation) + ast.posY;
        astSeg = {ast.posX, ast.posY, lastX, lastY, currentX, currentY};
        lastX = currentX, lastY = currentY;
        if (trianglesIntersect(shipL, astSeg) || trianglesIntersect(shipR, astSeg)) 
            return true;
    }
    
    return false;
}

bool GameState::pointsLieOnOneSide(Triangle const& tri1, Triangle const& tri2) const{
    // Based on this answer: https://stackoverflow.com/a/44269990

  float dXa = tri1.points[0] - tri2.points[4];
  float dYa = tri1.points[1] - tri2.points[5];
  float dXb = tri1.points[2] - tri2.points[4];
  float dYb = tri1.points[3] - tri2.points[5];
  float dXc = tri1.points[4] - tri2.points[4];
  float dYc = tri1.points[5] - tri2.points[5];
  float dX21 = tri2.points[4] - tri2.points[2];
  float dY12 = tri2.points[3] - tri2.points[5];
  float D = dY12 * (tri2.points[0] - tri2.points[4]) + dX21 * (tri2.points[1] - tri2.points[5]);
  float sa = dY12 * dXa + dX21 * dYa;
  float sb = dY12 * dXb + dX21 * dYb;
  float sc = dY12 * dXc + dX21 * dYc;
  float ta = (tri2.points[5] - tri2.points[1]) * dXa + (tri2.points[0] - tri2.points[4]) * dYa;
  float tb = (tri2.points[5] - tri2.points[1]) * dXb + (tri2.points[0] - tri2.points[4]) * dYb;
  float tc = (tri2.points[5] - tri2.points[1]) * dXc + (tri2.points[0] - tri2.points[4]) * dYc;
  if (D < 0) return ((sa >= 0 && sb >= 0 && sc >= 0) ||
                     (ta >= 0 && tb >= 0 && tc >= 0) ||
                     (sa+ta <= D && sb+tb <= D && sc+tc <= D));
  else return ((sa <= 0 && sb <= 0 && sc <= 0) ||
          (ta <= 0 && tb <= 0 && tc <= 0) ||
          (sa+ta >= D && sb+tb >= D && sc+tc >= D));
}

bool GameState::trianglesIntersect(Triangle const& tri1, Triangle const& tri2) const{
    return !(pointsLieOnOneSide(tri1, tri2) || pointsLieOnOneSide(tri2, tri1));
}

void GameState::drawShip() const{ 
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

// Starts a new game with number of asteroids of size 0, 1, 2, 3 according to input vector
void GameState::newGame(std::vector<unsigned int> const& astLayout){
    initialAsteroidField.clear();
    for (int i = 0 ; i < 4 ; ++i){
        if (i < astLayout.size()){
            initialAsteroidField.push_back(astLayout[i]);
        }
        else{
            initialAsteroidField.push_back(0);
        }
    }
    score = 0;
    wave = 0;
    GameObject::timeSinceLastUpdate = 0;
    ship = std::make_unique<Ship>(shipScale, winWidth/2.0f, winHeight/2.0f, 0.0f);

    asteroids.clear();
    srand(time(NULL));
    newWave();
    /*
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
    */
}

// Generates a field of asteroids that do not coincide with the ship
void GameState::newWave(){
    float xOffset = ship->posX, yOffset = ship->posY; // Ensure wave is centred on ship 
    for (unsigned int size = 0 ; size < 4 ; ++size){
        for (unsigned int i = 0 ; i < initialAsteroidField[size] + wave; ++i){
            float x = xOffset + winWidth * (0.16f + (rand() % 20) / 30.0f);
            float y = yOffset + winHeight * (0.16f + (rand() % 20) / 30.0f);
            float dir = 2 * piValue * (rand() % 60) / 60.0f;
            int rotSpeedModifier = (rand() % 5) - 2;
            if (!rotSpeedModifier) ++rotSpeedModifier;
            std::unique_ptr<Asteroid> tempAst = std::make_unique<Asteroid>(shipScale, x, y, dir, size, rotSpeedModifier);
            asteroids.push_back(std::move(tempAst));
        }
    }
    ++wave;
}