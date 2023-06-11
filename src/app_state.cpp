#include "../include/app_state.hpp"

AppState::AppState(unsigned int scale) : 
    winScale{scale},
    window(winWidth*winScale, winHeight*winScale), 
    context(window.getWindow(), winWidth*winScale, winHeight*winScale),
    gameState(winWidth, winHeight)
{
}

AppState::~AppState(){
}

void AppState::handleEvents(SDL_Event const&  event){
    switch(event.type){
        case SDL_QUIT:
            quit = true;
            break;
        //case SDL_WINDOWEVENT: // To do: handle window resize events
            //break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_F11:
                    window.toggleFullScreen();
                    break;
                case SDL_SCANCODE_LEFT:
                    gameState.ship.turnLeft(true);
                    break;
                case SDL_SCANCODE_RIGHT:
                    gameState.ship.turnRight(true);
                    break;
                case SDL_SCANCODE_UP:
                    gameState.ship.thrustForward(true);
                    break;
                case SDL_SCANCODE_DOWN:
                    gameState.ship.shootMissile(true);
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.scancode){
                case SDL_SCANCODE_LEFT:
                    gameState.ship.turnLeft(false);
                    break;
                case SDL_SCANCODE_RIGHT:
                    gameState.ship.turnRight(false);
                    break;
                case SDL_SCANCODE_UP:
                    gameState.ship.thrustForward(false);
                    break;
                case SDL_SCANCODE_DOWN:
                    gameState.ship.shootMissile(false);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void AppState::frame(unsigned int frameTime){ // Move to app state frame()
    // Clear buffer
            glClearColor(0.0f, 0.0f, 0.0f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Prepare to render
            gameState.wrapShader.useProgram();

            // Update ship transformation matrix
            glm::mat4 trans = gameState.ship.getTransMatrix(frameTime);
            glUniformMatrix4fv(gameState.uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));

            // Set draw colour
            glUniform4f(gameState.uniformColour, 0.5f, 0.6f, 1.0f, 1.0f);

            // Draw ship
            gameState.ship.draw();

            // Draw missiles
            glUniform4f(gameState.uniformColour, 0.3f, 0.4f, 1.0f, 1.0f);
            for (auto mis = gameState.ship.missiles.begin(); mis < gameState.ship.missiles.end(); /*no increment due to potential erasing*/){
                
                trans = (*mis)->getTransMatrix(frameTime);
                if ((*mis)->destroyThisFrame()){
                    mis = gameState.ship.missiles.erase(mis);
                }
                else{
                    glUniformMatrix4fv(gameState.uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
 
                    (*mis)->draw();
                    ++mis;
                }
            }
        
            // Set draw colour
            glUniform4f(gameState.uniformColour, 0.8f, 0.8f, 0.7f, 1.0f);
            //int count = 0;
            for (Asteroid& ast : gameState.asteroids)
            {
                //std::cout << "Drawing asteroid " <<count;count++;
                trans = ast.getTransMatrix(frameTime);
                //std::cout <<".";
                glUniformMatrix4fv(gameState.uniformModelTrans, 1, GL_FALSE, glm::value_ptr(trans));
                //std::cout <<".";
                ast.draw();
                //std::cout <<".\n";
            }    

            // Swap buffers
            SDL_GL_SwapWindow(window.getWindow());

            window.frame(frameTime);
}