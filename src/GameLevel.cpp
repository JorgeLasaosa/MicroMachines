
#include "GameLevel.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"

#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

#define SNOBEE_SPEED    0.1f//0.07f
#define PLAYER_SPEED    0.125f//0.125f

GLint scoreObj = 0;
GLint countLose = 0;


#define nullNode glm::vec2(-1,-1)

GameLevel::GameLevel(GLint numEggs) :
    field(15, std::vector<GameObject*>(13)), fieldStart(15, std::vector<GameObject*>(13)),
    deadEnemies(0), liveEnemies(0), state(LEVEL_START), showEggsCount(0), bonusOffset(0), numEggs(numEggs)
{
    genActualNode = nullNode;
    texScoreBonusWindow = ResourceManager::getTexture("pause-background");
    frScoreBonusWindow = SpriteFrame(texScoreBonusWindow.WIDTH, texScoreBonusWindow.HEIGHT, 128, 128, glm::vec2(0,0));
}


GameLevel::~GameLevel() {
    this->clear();
}

/**
 * Load data from level-file.
 */
void GameLevel::load(const std::string& filePath) {
    // Pengo
    creaturesTexture = ResourceManager::getTexture("creatures");
    eggsTexture = ResourceManager::getTexture("indicators-n-eggs");
    this->pengo = new Player(glm::vec2(6.5f,8.0f), glm::vec2(1,1), PLAYER_SPEED, creaturesTexture);
    this->pengo->configureFrame(160, 160, glm::vec2(0,0));

    // Walls
    Texture wallTexture = ResourceManager::getTexture("walls");
    Wallblock corner_ul = Wallblock(glm::vec2(0.0f, 1.5f), glm::vec2(0.5f, 0.5f), wallTexture);
    corner_ul.configureFrame(80,80,glm::vec2(0,0));
    Wallblock corner_ur = Wallblock(glm::vec2(13.5f, 1.5f), glm::vec2(0.5f, 0.5f), wallTexture);
    corner_ur.configureFrame(80,80,glm::vec2(0,1));
    Wallblock corner_dl = Wallblock(glm::vec2(0.0f, 17.0f), glm::vec2(0.5f, 0.5f), wallTexture);
    corner_dl.configureFrame(80,80,glm::vec2(0,0));
    Wallblock corner_dr = Wallblock(glm::vec2(13.5f, 17.0f), glm::vec2(0.5f, 0.5f), wallTexture);
    corner_dr.configureFrame(80,80,glm::vec2(0,1));
    wallN.push_back(corner_ul);
    wallW.push_back(corner_ul);
    wallE.push_back(corner_ur);
    wallS.push_back(corner_dl);
    for (int i=1; i<27; i++) {
        Wallblock wall_n = Wallblock(glm::vec2(i * 0.5f, 1.5f), glm::vec2(0.5f, 0.5f), wallTexture);
        wall_n.configureFrame(80,80,glm::vec2(0,1));
        wallN.push_back(wall_n);

        Wallblock wall_s = Wallblock(glm::vec2(i * 0.5f, 17.0f), glm::vec2(0.5f, 0.5f), wallTexture);
        wall_s.configureFrame(80,80,glm::vec2(0,1));
        wallS.push_back(wall_s);
    }

    for (int i=0; i<30; i++) {
        Wallblock wall_e = Wallblock(glm::vec2(13.5f, 2.0f + i * 0.5f), glm::vec2(0.5f,0.5f), wallTexture);
        wall_e.configureFrame(80,80,glm::vec2(0,0));
        wallE.push_back(wall_e);

        Wallblock wall_w = Wallblock(glm::vec2(0, 2.0f + i * 0.5f), glm::vec2(0.5f,0.5f), wallTexture);
        wall_w.configureFrame(80,80,glm::vec2(0,0));
        wallW.push_back(wall_w);
    }
    wallN.push_back(corner_ur);
    wallW.push_back(corner_dl);
    wallE.push_back(corner_dr);
    wallS.push_back(corner_dr);

    // Blocks
    Texture blockTexture = ResourceManager::getTexture("blocks");
    std::ifstream file(filePath);
    std::string line;
    GLuint n;
    int i = 0, j = 0, numIceBlocks;
    if (file) {
        while(std::getline(file, line)) {
            std::istringstream iss(line);
            while(iss >> n) {
                if (n == 1) {
                    // Ice blocks
                    field[i][j] = new Iceblock(glm::vec2(0.5f + j, 2.0f + i), glm::vec2(1,1), 0.375f, blockTexture);
                    field[i][j]->configureFrame(160, 160, glm::vec2(0,0));
                    numIceBlocks++;
                }
                else if (n == 2) {
                    // Diamond blocks
                    field[i][j] = new Diamondblock(glm::vec2(0.5f + j, 2.0f + i), glm::vec2(1,1), 0.375f, blockTexture);
                    field[i][j]->configureFrame(160, 160, glm::vec2(0,1));
                } else {
                    // Empty places. (Ghost-blocks to generate level)
                    fieldStart[i][j] = new Iceblock(glm::vec2(0.5f + j, 2.0f + i), glm::vec2(1,1), 0.375f, blockTexture);
                    fieldStart[i][j]->configureFrame(160, 160, glm::vec2(0,0));
                }
                j++;
            }
            j=0;
            i++;
        }
    }

    // Select [numEggs] random egg blocks
    for(GLint i = 0; i < numEggs; i++){
        bool selected = false;
        while(!selected) {
            GLint row = (rand() % 15);
            GLint column = (rand() % 13);
            Iceblock* block = dynamic_cast<Iceblock*>(field[row][column]);
            if(block!=nullptr && !block->isEggBlock) {
                block->isEggBlock = true;
                //block->changeIndexFrame(glm::vec2(3,0));
                eggBlocks.push_back(block);
                selected = true;
            }
        }
    }
}

/**
 * Simulates progressive level generation deleting the ghost blocks from the corridors
 */
bool GameLevel::generate() {
    bool end = false;
    if (genActualNode==nullNode && mazeNodesStart.size() == 0) {
        // No more neightbour candidates. Look for the next candidate from down to up and from left to right in the map
        end = true;
        for(int i = 14; i >=0 && end; i--) {
            for(int j = 0; j < 12 && end; j++) {
                if (fieldStart[i][j]!=nullptr) {
                    // Candidate found
                    end = false;
                    mazeNodesStart.push(glm::vec2(i,j));
                }
            }
        }
    }
    if(!end) {
        // Look postponed candidates
        if(genActualNode==nullNode){
            genActualNode = mazeNodesStart.front();
            mazeNodesStart.pop();
        }
        int n_x = genActualNode.x, n_y = genActualNode.y;
        int numNeightbours = 0;
        delete fieldStart[n_x][n_y];
        fieldStart[n_x][n_y] = nullptr;
        genActualNode = nullNode;


        // Check up
        if(n_y>0 && fieldStart[n_x][n_y-1] != nullptr) {
            // Set next candidate
            genActualNode = glm::vec2(n_x,n_y-1);
            numNeightbours++;
        }

        // Check left
        if(n_x>0 && fieldStart[n_x-1][n_y] != nullptr) {
            if (numNeightbours==0) {
                // Set next candidate
                genActualNode = glm::vec2(n_x-1,n_y);
            } else {
                // Postpone candidate
                mazeNodesStart.push(glm::vec2(n_x-1,n_y));
            }
            numNeightbours++;
        }

        // Check right
        if(n_x<14 && fieldStart[n_x+1][n_y] != nullptr) {
            if (numNeightbours==0) {
                // Set next candidate
                genActualNode = glm::vec2(n_x+1,n_y);
            } else {
                // Postpone candidate
                mazeNodesStart.push(glm::vec2(n_x+1,n_y));
            }
            numNeightbours++;
        }

        // Check down
        if(n_y<12 && fieldStart[n_x][n_y+1] != nullptr) {
            if (numNeightbours==0) {
                // Set next candidate
                genActualNode = glm::vec2(n_x,n_y+1);
            } else {
                // Postpone candidate
                mazeNodesStart.push(glm::vec2(n_x,n_y+1));
            }
            numNeightbours++;
        }
    }
    return end;
}

/**
 * Draw level elements in 2D
 */
void GameLevel::draw(SpriteRenderer& renderer) {

    for (GLuint i = 0; i < wallN.size(); i++) {
        wallN[i].draw(renderer);
        wallS[i].draw(renderer);
    }

    for (GLuint i = 0; i < wallE.size(); i++) {
        wallE[i].draw(renderer);
        wallW[i].draw(renderer);
    }

    for (auto &i : field) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(renderer);
            }
        }
    }

    if (state != LEVEL_LOSE2){
        for (auto &i : enemies) {
            if (i != nullptr) {
                i->draw(renderer);
            }
        }
        for (auto &i : eggs) {
            if (i != nullptr) {
                i->draw(renderer);
            }
        }
    }

    for (auto &i : floatingTexts) {
        if (i != nullptr) {
            i->draw();
        }
    }

    pengo->draw(renderer);

    if(state == LEVEL_BONUS && bonusOffset>50) {
        renderer.drawSprite(texScoreBonusWindow, glm::vec2(3.0f, 7.5f), glm::vec2(8.0f, 2.5f), frScoreBonusWindow);
        ResourceManager::textRenderer->renderText("BONUS", glm::vec2(3.5f, 8.0f), 0.5f, glm::vec3(1,1,0));
        ResourceManager::textRenderer->renderText("PTS.", glm::vec2(8.5f, 9.0f), 0.5f, glm::vec3(1, 0.7019f, 0.8431f));
        std::ostringstream strs;
        GLint numDigits = 1;
        GLint tmpScore = scoreObj - Game::score;
        while (tmpScore>=10) {
            tmpScore = tmpScore/10;
            numDigits++;
        }
        while(numDigits<5) {
            strs << " ";
            numDigits++;
        }
        strs << (scoreObj - Game::score);
        std::string str = strs.str();
        ResourceManager::textRenderer->renderText(str, glm::vec2(6.0f, 8.5f), 0.5f, glm::vec3(255, 255, 255));
    }
}

/*
 * Draw level elements in 3D
 */
void GameLevel::draw(Component3D* component3D, Cube3DRenderer& cube3DRenderer) {
    for (GLuint i = 0; i < wallN.size(); i++) {
        wallN[i].draw(cube3DRenderer, -0.25f);
        wallS[i].draw(cube3DRenderer, -0.25f);
    }

    for (GLuint i = 0; i < wallE.size(); i++) {
        wallE[i].draw(cube3DRenderer, -0.25f);
        wallW[i].draw(cube3DRenderer, -0.25f);
    }

    for (auto &i : field) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(cube3DRenderer);
            }
        }
    }

//    if (state != LEVEL_LOSE2){
//        for (auto &i : enemies) {
//            if (i != nullptr) {
//                i->draw(renderer);
//            }
//        }
//        for (auto &i : eggs) {
//            if (i != nullptr) {
//                i->draw(renderer);
//            }
//        }
//    }
//
//    for (auto &i : floatingTexts) {
//        if (i != nullptr) {
//            i->draw();
//        }
//    }
//
//    pengo->draw(renderer);
//
//    if(state == LEVEL_BONUS && bonusOffset>50) {
//        renderer.drawSprite(texScoreBonusWindow, glm::vec2(3.0f, 7.5f), glm::vec2(8.0f, 2.5f), frScoreBonusWindow);
//        ResourceManager::textRenderer->renderText("BONUS", glm::vec2(3.5f, 8.0f), 0.5f, glm::vec3(1,1,0));
//        ResourceManager::textRenderer->renderText("PTS.", glm::vec2(8.5f, 9.0f), 0.5f, glm::vec3(1, 0.7019f, 0.8431f));
//        std::ostringstream strs;
//        GLint numDigits = 1;
//        GLint tmpScore = scoreObj - Game::score;
//        while (tmpScore>=10) {
//            tmpScore = tmpScore/10;
//            numDigits++;
//        }
//        while(numDigits<5) {
//            strs << " ";
//            numDigits++;
//        }
//        strs << (scoreObj - Game::score);
//        std::string str = strs.str();
//        ResourceManager::textRenderer->renderText(str, glm::vec2(6.0f, 8.5f), 0.5f, glm::vec3(255, 255, 255));
//    }
}

/*
 * Param [to] indicates the number of field rows to clean.
 * Clears the [to] first rows of the field
 */
void GameLevel::clearFromTop(SpriteRenderer& renderer, GLfloat to) {
    for(int i = 0; i < wallN.size(); i++) {
        if (to <= 0.0f) {
            wallN[i].draw(renderer);
        }
        if(to < 17) {
            wallS[i].draw(renderer);
        }

    }
    for (int i = 0; i < wallW.size(); i++) {
        if (wallW[i].position.y > to) {
            wallW[i].draw(renderer);
            wallE[i].draw(renderer);
        }
    }
    for (auto& i : field) {
        for (GameObject* j : i) {
            if (j != nullptr) {
                if (j->position.y > to) {
                    j->draw(renderer);
                }
            }
        }
    }
}

/**
 * Draw elements while generating in 2D
 */
void GameLevel::drawGenerating(SpriteRenderer& renderer) {
    for (GLuint i = 0; i < wallN.size(); i++) {
        wallN[i].draw(renderer);
        wallS[i].draw(renderer);
    }

    for (GLuint i = 0; i < wallE.size(); i++) {
        wallE[i].draw(renderer);
        wallW[i].draw(renderer);
    }

    for (auto &i : field) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(renderer);
            }
        }
    }

    for (auto &i : fieldStart) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(renderer);
            }
        }
    }
}

/**
 * Draw elements while generating in 3D
 */
void GameLevel::drawGenerating(Cube3DRenderer& cube3DRenderer) {
    for (GLuint i = 0; i < wallN.size(); i++) {
        wallN[i].draw(cube3DRenderer, -0.25f);
        wallS[i].draw(cube3DRenderer, -0.25f);
    }

    for (GLuint i = 0; i < wallE.size(); i++) {
        wallE[i].draw(cube3DRenderer, -0.25f);
        wallW[i].draw(cube3DRenderer, -0.25f);
    }

    for (auto &i : field) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(cube3DRenderer);
            }
        }
    }

    for (auto &i : fieldStart) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(cube3DRenderer);
            }
        }
    }
}

/**
 * Check if there is a block or a wall at position 'pos' in the map.
 *
 * Note: Not to confuse with GameObject::overlap()
 */
bool GameLevel::checkCollision(glm::vec2 pos) const {
    int j = pos.x - 0.5f;
    int i = pos.y - 2;
    if (i>=15 || i < 0 || j>=13 || j < 0) return true;
    return field[i][j]!=nullptr && field[i][j]->state!=MOVING;
}

/**
 * Check if there is a wall at position 'pos' in the map.

 */
bool GameLevel::checkWalls(glm::vec2 pos) const {
    int j = pos.x - 0.5f;
    int i = pos.y - 2;
    if (i>=15 || i < 0 || j>=13 || j < 0) return true;
    return false;
}

/**
 * Returns the block from a position in the map
 */
GameObject* GameLevel::getObjFromPosition(glm::vec2 pos) const {
    int j = pos.x - 0.5f;
    int i = pos.y - 2;
    if (i>=15 || i < 0 || j>=13 || j < 0) return nullptr;
    return field[i][j];
}

/**
 * Move active blocks and manage its logic (stop sliding, collision with a SNO-BEE...)
 */
void GameLevel::moveBlocks(GLfloat interpolation) {
    for (std::vector< GameObject* >::iterator it = activeObjects.begin() ; it != activeObjects.end(); it++) {
        if((*it)==nullptr){
            // Delete from vector
        } else if(!(*it)->move(interpolation)){
            if ((*it)->killing>0) {
                ResourceManager::soundEngine->play2D("sounds/snow-bee-squashed.wav", false);
                liveEnemies-=(*it)->killing;
                deadEnemies+=(*it)->killing;
                if((*it)->killing==1){
                    Game::score += 400;
                    floatingTexts.push_back(new FloatingText((*it)->position + glm::vec2(0.0f,0.3f), "400", 50, 0.33, glm::vec3(1.0f,1.0f,1.0f)));
                } else if((*it)->killing==2){
                    Game::score += 1600;
                    floatingTexts.push_back(new FloatingText((*it)->position + glm::vec2(0.0f,0.37f), "1600", 50, 0.25, glm::vec3(1.0f,1.0f,1.0f)));
                } else if((*it)->killing==3){
                    Game::score += 3200;
                    floatingTexts.push_back(new FloatingText((*it)->position + glm::vec2(0.0f,0.37f), "3200", 50, 0.25, glm::vec3(1.0f,1.0f,1.0f)));
                }
                (*it)->killing = 0;
            } else {
                ResourceManager::soundEngine->play2D("sounds/block-stopped.wav", false);
            }

            // Update position
            int jor = (*it)->origPos.x - 0.5f;
            int ior = (*it)->origPos.y - 2;
            int j = (*it)->destination.x - 0.5f;
            int i = (*it)->destination.y - 2;
            field[i][j] = field[ior][jor];
            field[ior][jor] = nullptr;
            (*it)->origPos = (*it)->position;


            // Check 3 diamond blocks in line
            Diamondblock* block = dynamic_cast<Diamondblock*>(field[i][j]);
            GLint lineTamH = 0;
            GLint lineTamV = 0;
            if(block!=nullptr) {
                lineTamH++;
                lineTamV++;
                // Check horizontal
                if(j>0){
                    if (dynamic_cast<Diamondblock*>(field[i][j-1])!=nullptr) {
                        lineTamH++;
                    }
                    if(j>1){
                        if (dynamic_cast<Diamondblock*>(field[i][j-2])!=nullptr) {
                            lineTamH++;
                        }
                    }
                }
                if(j<12){
                    if (dynamic_cast<Diamondblock*>(field[i][j+1])!=nullptr) {
                        lineTamH++;
                    }
                    if(j<11){
                        if (dynamic_cast<Diamondblock*>(field[i][j+2])!=nullptr) {
                            lineTamH++;
                        }
                    }
                }

                // Check vertical
                if(i>0){
                    if (dynamic_cast<Diamondblock*>(field[i-1][j])!=nullptr) {
                        lineTamV++;
                    }
                    if(i>1){
                        if (dynamic_cast<Diamondblock*>(field[i-2][j])!=nullptr) {
                            lineTamV++;
                        }
                    }
                }
                if(i<14){
                    if (dynamic_cast<Diamondblock*>(field[i+1][j])!=nullptr) {
                        lineTamV++;
                    }
                    if(i<13){
                        if (dynamic_cast<Diamondblock*>(field[i+2][j])!=nullptr) {
                            lineTamV++;
                        }
                    }
                }
            }

            if (lineTamV==3 || lineTamH==3) {
                // BONUS MODE!
                state = LEVEL_BONUS;
            }

            // Set not moving block
            (*it) = nullptr;
        } else {
            for (auto &i : enemies) {
                // If it collides with a SNO-BEE, slide and kill him
                if(i!=nullptr && (*it)->overlaps(i) && i->state != DYING && i->state != DEAD) {
                    i->state = DYING;
                    (*it)->killing++;
                    switch((*it)->movement) {
                        case MOVE_DOWN:
                        case MOVE_UP:
                            i->setDestination(glm::vec2(i->getPosition().x,(*it)->getDestination().y));
                        break;
                        case MOVE_LEFT:
                        case MOVE_RIGHT:
                            i->setDestination(glm::vec2((*it)->getDestination().x,i->getPosition().y));
                        break;
                    }
                    i->lastDist = -1;
                    i->movement = (*it)->movement;
                    i->velocity = (*it)->velocity;
                }
            }
        }
    }
}

void GameLevel::moveEnemies(GLfloat interpolation) {
    // northTaken, southTaken, eastTaken, westTaken
    GLboolean positionsTaken[4] = {false, false, false, false};
    for (std::vector< Snobee* >::iterator it = enemies.begin() ; it != enemies.end(); it++) {
        if((*it) != nullptr){
            if ((*it)->state == STOPPED) {
                // GLfloat p = (GLfloat) rand() / RAND_MAX;
                // if (p > 0.8) {  // 20% de probabilidad de perseguir
                //    (*it)->nextMovePursuit(this);
                // }
                // else {
                //    if((*it)->nextMoveRandom(this) == 0){
                //        (*it)->nextMoveRandom(this, true);
                //    }
                // }
                (*it)->nextMovePursuit(this, positionsTaken);

                // Check shaking walls
                if ((*it)->position.x == 0.5f && wallW[0].shaking>0) {
                    (*it)->numb();
                    ResourceManager::soundEngine->play2D("sounds/snow-bee-stunned.wav", false);
                } else if ((*it)->position.x == 12.5f && wallE[0].shaking>0) {
                    (*it)->numb();
                    ResourceManager::soundEngine->play2D("sounds/snow-bee-stunned.wav", false);
                } else if ((*it)->position.y == 2 && wallN[0].shaking>0) {
                    (*it)->numb();
                    ResourceManager::soundEngine->play2D("sounds/snow-bee-stunned.wav", false);
                } else if ((*it)->position.y == 16 && wallS[0].shaking>0) {
                    (*it)->numb();
                    ResourceManager::soundEngine->play2D("sounds/snow-bee-stunned.wav", false);
                }

                // Check if it's numb
                if ((*it)->isNumb) {
                    (*it)->state = NUMB;
                }
            }

            if ((*it)->state == MOVING || (*it)->state == DESTROYING) {
                (*it)->setFrameHandler((*it)->getFrameHandler() + interpolation);
                GLint isDestroying = 0;
                if ((*it)->getFrameHandler() > 4) {
                    (*it)->setFrameHandler(0);
                    (*it)->setFrameIndex(((*it)->getFrameIndex()+1) % 2);
                    GLint orientation = 0;
                    switch((*it)->movement) {
                        case MOVE_UP: orientation = 2;
                        break;
                        case MOVE_DOWN: orientation = 0;
                        break;
                        case MOVE_LEFT: orientation = 1;
                        break;
                        case MOVE_RIGHT: orientation = 3;
                        break;
                    }
                    SpriteFrame* frame = (*it)->getSpriteFrame();
                    if ((*it)->state == DESTROYING) isDestroying = 1;
                    frame->setIndex(frame->getIndexOrig() + glm::vec2(orientation*2 + (*it)->getFrameIndex(),isDestroying));
                }
                (*it)->move(interpolation);
            }

            if ((*it)->state == NUMB) {
                (*it)->setFrameHandler((*it)->getFrameHandler() + interpolation);
                if (((GLint) (*it)->getFrameHandler()) > 2) {
                    (*it)->setFrameHandler(0);
                    (*it)->setFrameIndex((*it)->getFrameIndex()+1);
                    SpriteFrame* frame = (*it)->getSpriteFrame();
                    frame->setIndex(frame->getIndexOrig() + glm::vec2(6 + (*it)->getFrameIndex()%2,-1));
                    if((*it)->getFrameIndex()==25) {
                        (*it)->state = STOPPED;
                        (*it)->numb(false);
                        (*it)->setFrameIndex(0);
                    }
                }
            }

            if ((*it)->state == DYING) {
                GLint orientation = 0;
                switch((*it)->movement) {
                    case MOVE_UP: orientation = 0;
                    break;
                    case MOVE_DOWN: orientation = 2;
                    break;
                    case MOVE_LEFT: orientation = 3;
                    break;
                    case MOVE_RIGHT: orientation = 1;
                    break;
                }
                SpriteFrame* frame = (*it)->getSpriteFrame();
                frame->setIndex(frame->getIndexOrig() + glm::vec2(orientation*2,0));
                if (!(*it)->move(interpolation)) {
                    (*it)->state = DEAD;
                    (*it)->setFrameIndex(0);
                    (*it)->setFrameHandler(0);
                }
            }

            if ((*it)->state == DEAD) {
                (*it)->setFrameHandler((*it)->getFrameHandler() + interpolation);
                if (((GLint) (*it)->getFrameHandler()) > 2) {
                    (*it)->setFrameHandler(0);
                    (*it)->setFrameIndex(((*it)->getFrameIndex()+1) % 2);
                    GLint orientation = 0;
                    switch((*it)->movement) {
                        case MOVE_UP: orientation = 0;
                        break;
                        case MOVE_DOWN: orientation = 2;
                        break;
                        case MOVE_LEFT: orientation = 3;
                        break;
                        case MOVE_RIGHT: orientation = 1;
                        break;
                    }
                    SpriteFrame* frame = (*it)->getSpriteFrame();
                    frame->setIndex(frame->getIndexOrig() + glm::vec2(orientation*2 + (*it)->getFrameIndex(),3));
                    if((*it)->getFrameIndex()==0) {
                        (*it) = nullptr;
                    }
                }
            }
        }
    }
}

void GameLevel::destroyBlocks(GLfloat interpolation) {
    for (std::vector< Iceblock* >::iterator it = deadBlocks.begin() ; it != deadBlocks.end(); it++) {
        if((*it)==nullptr){
            //activeObjects.erase(it);
            //it = activeObjects.begin();
        } else {
            (*it)->keepDisintegrate(interpolation);
            if ((*it)->state==DEAD) {
                int j = (*it)->position.x - 0.5f;
                int i = (*it)->position.y - 2;
                if ((*it)->destroyByPengo && (*it)->isEggBlock) {
                    deadEnemies++;
                    Game::score += 500;
                    // TODO delay sound
                    ResourceManager::soundEngine->play2D("sounds/snow-bee-egg-destroyed.wav", false);
                    floatingTexts.push_back(new FloatingText((*it)->position + glm::vec2(0.0f,0.3f), "500", 50, 0.33, glm::vec3(1.0f,1.0f,1.0f)));
                }
                field[i][j] = nullptr;
                (*it) = nullptr;
            }
        }
    }
}

void GameLevel::update() {
    if (state==LEVEL_PLAY || state==LEVEL_SHOWING_EGGS){
        for (std::vector< SnobeeEgg* >::iterator it = eggs.begin() ; it != eggs.end(); it++) {
            if((*it) != nullptr){
                if(!(*it)->update(this)) {
                    (*it) = nullptr;
                }
            }
        }


        for (auto &i : enemies) {
            if (i != nullptr) {
                i->update();
            }
        }

        for (std::vector< FloatingText* >::iterator it = floatingTexts.begin() ; it != floatingTexts.end(); it++) {
            if((*it) != nullptr){
                if(!(*it)->update()) {
                    (*it) = nullptr;
                }
            }
        }

        for (GLuint i = 0; i < wallN.size(); i++) {
            wallN[i].update();
            wallS[i].update();
        }

        for (GLuint i = 0; i < wallE.size(); i++) {
            wallE[i].update();
            wallW[i].update();
        }

        while(liveEnemies < 3 && deadEnemies<=numEggs-3) {
            Iceblock* eggblock = eggBlocks.back();
            eggBlocks.pop_back();
            eggblock->disintegrate(this, false);
            // Create SnoBee Egg
            SnobeeEgg* egg = new SnobeeEgg(eggblock->getPosition(), glm::vec2(1,1), SNOBEE_SPEED, eggsTexture, GREEN);//glm::vec2(0.5f, 2.0f)
            egg->configureFrame(160, 160, glm::vec2(0,4));
            this->eggs.push_back(egg);
            state = LEVEL_SHOWING_EGGS;

            liveEnemies++;
        }
        if (deadEnemies == numEggs) {
            // WIN LEVEL
            state = LEVEL_WIN;
        }
    }
    if(state==LEVEL_SHOWING_EGGS) {
        if (showEggsCount%15 == 0) {
            for (auto &i : eggBlocks) {
                if (i!=nullptr) {
                    i->changeIndexFrame(glm::vec2(0,0));
                    if (showEggsCount>40) {
                        showEggsCount = 0;
                        state = LEVEL_PLAY;
                    }
                }
            }
        }
        if ((showEggsCount+7)%15 == 0) {
            for (auto &i : eggBlocks) {
                if (i!=nullptr) {
                    i->changeIndexFrame(glm::vec2(1,0));
                }
            }
        }
        showEggsCount++;
    }

    if (state == LEVEL_BONUS) {
        if (bonusOffset==0) {
            scoreObj = Game::score + 10000;// TODO Menos si al lado de pared!
            ResourceManager::musicEngine->stopAllSounds();
            ResourceManager::musicEngine->play2D("sounds/diamond-blocks-lined-up.wav", true);
        }
        bonusOffset++;
        if (bonusOffset<50) {
            for (GLuint i = 0; i < wallN.size(); i++) {
                wallN[i].changeIndexFrame(glm::vec2(((GLint)(bonusOffset+i)/2)%8, 2));
                wallS[i].changeIndexFrame(glm::vec2(((GLint)(bonusOffset-i+wallW.size()+1)/2)%8, 2));
            }
            for (GLuint i = 0; i < wallE.size(); i++) {
                wallE[i].changeIndexFrame(glm::vec2((((GLint)(bonusOffset+i+wallN.size()-1))/2)%8, 2));
                wallW[i].changeIndexFrame(glm::vec2(((GLint)(bonusOffset-i+wallW.size())/2)%8, 2));
            }
            for (auto &i : field) {
                for (auto &j : i) {
                    if (j != nullptr) {
                        Iceblock* block = dynamic_cast<Iceblock*>(j);
                        if (block != nullptr) {
                            block->changeIndexFrame(glm::vec2(((GLint) bonusOffset/2)%9, 0));
                        }
                    }
                }
            }
        }
        if (bonusOffset==50) {
            ResourceManager::musicEngine->stopAllSounds();
        }
        if (bonusOffset==70) {
            ResourceManager::musicEngine->play2D("sounds/counting-bonus-points.wav", true);
        }
        if (bonusOffset>=70 && Game::score < scoreObj) {
            Game::score += 100;
        }
        if (bonusOffset==170) {//bonusOffset>150
            ResourceManager::musicEngine->stopAllSounds();
        }
        if (bonusOffset>200) {
            for (auto &i : enemies) {
                if (i != nullptr) {
                    i->numb();
                }
            }
            for (GLuint i = 0; i < wallN.size(); i++) {
                wallN[i].changeIndexFrame(wallN[i].getSpriteFrame()->getIndexOrig());
                wallS[i].changeIndexFrame(wallS[i].getSpriteFrame()->getIndexOrig());
            }
            for (GLuint i = 0; i < wallE.size(); i++) {
                wallE[i].changeIndexFrame(wallE[i].getSpriteFrame()->getIndexOrig());
                wallW[i].changeIndexFrame(wallW[i].getSpriteFrame()->getIndexOrig());
            }
            for (auto &i : field) {
                for (auto &j : i) {
                    if (j != nullptr) {
                        Iceblock* block = dynamic_cast<Iceblock*>(j);
                        if (block != nullptr) {
                            block->changeIndexFrame(glm::vec2(0, 0));
                        }
                    }
                }
            }

            bonusOffset = 0;
            ResourceManager::musicEngine->play2D("sounds/level.wav", true);
            state = LEVEL_PLAY;
        }
    }

    if (state == LEVEL_LOSE) {
        countLose++;
        if (countLose>=50) {
            countLose = 0;
            state = LEVEL_LOSE2;
            pengo->state = DEAD;
            ResourceManager::musicEngine->play2D("sounds/miss.wav", false);
        }
    }
    if (state == LEVEL_LOSE2) {
        countLose++;
        if (!ResourceManager::musicEngine->isCurrentlyPlaying("sounds/miss.wav")) {
            state = LEVEL_TMP;
            countLose = 0;
        } else {
            if (countLose%4 == 0){
                pengo->changeIndexFrame(glm::vec2(((GLint) pengo->getSpriteFrame()->getIndex().x + 1) % 2, 2));
            }
        }
    }
}

/*
 * Deletes dead Pengo and creates a new Pengo in starting position
 */
void GameLevel::respawnPengo() {
    delete pengo;
    glm::vec2 newPosition = nearestAvailablePosition(6, 6) + glm::vec2(2.0f, 0.5f);
    this->pengo = new Player(glm::vec2(newPosition.y,newPosition.x), glm::vec2(1,1), PLAYER_SPEED, creaturesTexture);
    this->pengo->configureFrame(160, 160, glm::vec2(0,0));
}

/*
 * Auxiliar method to respawnEnemiesAtCorners
 * Returns the nearest available position from field[row][col]
 */
glm::vec2 GameLevel::nearestAvailablePosition(GLint row, GLint col) const {
    GLint radius = 0;
    GLint rowToCheck, colToCheck;
    while(true) {
        for (int y = -radius; y <= radius; y++) {
            rowToCheck = row + y;
            colToCheck = col + radius;
            if (rowToCheck >= 0 && rowToCheck < 15 && colToCheck >= 0 && colToCheck < 13) {
                if (field[rowToCheck][colToCheck] == nullptr) {
                    return glm::vec2(rowToCheck, colToCheck);
                }
            }
            colToCheck = col - radius;
            if (rowToCheck >= 0 && rowToCheck < 15 && colToCheck >= 0 && colToCheck < 13) {
                if (field[rowToCheck][colToCheck] == nullptr) {
                    return glm::vec2(rowToCheck, colToCheck);
                }
            }
        }
        for (int x = -radius+1; x <= radius-1; x++) {
            colToCheck = col + x;
            rowToCheck = row + radius;
            if (rowToCheck >= 0 && rowToCheck < 15 && colToCheck >= 0 && colToCheck < 13) {
                if (field[rowToCheck][colToCheck] == nullptr) {
                    return glm::vec2(rowToCheck, colToCheck);
                }
            }
            rowToCheck = row - radius;
            if (rowToCheck >= 0 && rowToCheck < 15 && colToCheck >= 0 && colToCheck < 13) {
                if (field[rowToCheck][colToCheck] == nullptr) {
                    return glm::vec2(rowToCheck, colToCheck);
                }
            }
        }
        radius++;
    }
}
/*
 * Respawn enemies at corners after PENGO's death
 */
void GameLevel::respawnEnemiesAtCorners() {
    std::queue<glm::vec2> corners;
    corners.push(glm::vec2(14, 0));
    corners.push(glm::vec2(0, 12));
    corners.push(glm::vec2(14, 12));
    corners.push(glm::vec2(0, 0));

    for(Snobee* e : enemies) {
        delete e;
    }
    for(SnobeeEgg* e : eggs) {
        delete e;
    }
    enemies.clear();
    eggs.clear();
    for (int i = 0; i < liveEnemies; i++) {
        glm::vec2 newPosition = nearestAvailablePosition(corners.front().x, corners.front().y) + glm::vec2(2.0f, 0.5f);
        Snobee* newSnobee = new Snobee(glm::vec2(newPosition.y, newPosition.x), glm::vec2(1.0f,1.0f), SNOBEE_SPEED, creaturesTexture, GREEN);
        newSnobee->configureFrame(160, 160, glm::vec2(0,9));
        enemies.push_back(newSnobee);
        corners.pop();
    }
}

/**
 * Restarts the state of the blocks to the last STOPPED state.
 */
void GameLevel::respawnBlocks() {
    for (auto &i : field) {
        for (auto &j : i) {
            // Restart the block state
            if (j != nullptr) {
                j->state = STOPPED;
                j->killing = 0;
                j->position = j->origPos;
                j->changeIndexFrame(j->getSpriteFrame()->getIndexOrig());
            }
        }
    }

    // Restart wall states
    for (GLuint i = 0; i < wallN.size(); i++) {
        wallN[i].changeIndexFrame(wallN[i].getSpriteFrame()->getIndexOrig());
        wallN[i].shaking = -1;
        wallS[i].changeIndexFrame(wallS[i].getSpriteFrame()->getIndexOrig());
        wallS[i].shaking = -1;
    }

    for (GLuint i = 0; i < wallE.size(); i++) {
        wallE[i].changeIndexFrame(wallE[i].getSpriteFrame()->getIndexOrig());
        wallE[i].shaking = -1;
        wallW[i].changeIndexFrame(wallW[i].getSpriteFrame()->getIndexOrig());
        wallW[i].shaking = -1;
    }

    activeObjects.clear();
    deadBlocks.clear();
    floatingTexts.clear();
}

void GameLevel::clear() {
    delete pengo;
    wallN.clear();
    wallS.clear();
    wallE.clear();
    wallW.clear();

    for(auto& i : enemies) {
        delete i;
    }
    enemies.clear();
    activeObjects.clear();
    deadBlocks.clear();
    eggBlocks.clear();
    floatingTexts.clear();

    fieldStart.clear();
    for(auto &i : field) {
        for (auto &j : i) {
            delete j;
        }
        i.clear();
    }
}

