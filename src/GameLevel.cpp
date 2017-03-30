
#include "GameLevel.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"

#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

GLint scoreObj = 0;
GLint countLose = 0;

#define nullNode glm::vec2(-1,-1)

GameLevel::GameLevel():field(15, std::vector<GameObject*>(13)),fieldStart(15, std::vector<GameObject*>(13)), deadEnemies(0), liveEnemies(0), state(LEVEL_START),
    showEggsCount(0), bonusOffset(0) {
    genActualNode = nullNode;
    /* initialize random seed: */
    srand (time(NULL));
}


GameLevel::~GameLevel() {
    this->clear();
}

/**
 * Load data from level-file.
 */
void GameLevel::load(const GLchar* filePath) {
    // Pengo
    creaturesTexture = ResourceManager::getTexture("creatures");
    eggsTexture = ResourceManager::getTexture("indicators-n-eggs");
    this->pengo = new Player(glm::vec2(6.5f,8.0f), glm::vec2(1,1), 0.125f, creaturesTexture);
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

    // Select 6 random egg blocks
    for(GLint i = 0; i < 6; i++){
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
 * Draw level elements
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
}

/**
 * Draw elements while generating
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


    for (auto &i : fieldStart) {
        for (auto &j : i) {
            if (j != nullptr) {
                j->draw(renderer);
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
            } else {
                ResourceManager::soundEngine->play2D("sounds/block-stopped.wav", false);
            }

            // Update position
            (*it)->killing = 0;
            int jor = (*it)->origPos.x - 0.5f;
            int ior = (*it)->origPos.y - 2;
            int j = (*it)->destination.x - 0.5f;
            int i = (*it)->destination.y - 2;
            field[i][j] = field[ior][jor];
            field[ior][jor] = nullptr;


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
                    i->movement = (*it)->movement;
                    i->velocity = (*it)->velocity;
                }
            }
        }
    }
}

void GameLevel::moveEnemies(GLfloat interpolation) {
    for (std::vector< Snobee* >::iterator it = enemies.begin() ; it != enemies.end(); it++) {
        if((*it) != nullptr){
            int numMovs = 0;
            if ((*it)->state == STOPPED) {
                // Next random pos
                std::vector< Move > movsPosibles;
                if(!this->checkCollision((*it)->position + glm::vec2(1,0)) && (*it)->movement!=MOVE_LEFT) {
                    movsPosibles.push_back(MOVE_RIGHT);
                    numMovs++;
                }
                if(!this->checkCollision((*it)->position + glm::vec2(-1,0)) && (*it)->movement!=MOVE_RIGHT) {
                    movsPosibles.push_back(MOVE_LEFT);
                    numMovs++;
                }
                if(!this->checkCollision((*it)->position + glm::vec2(0,1)) && (*it)->movement!=MOVE_UP) {
                    movsPosibles.push_back(MOVE_DOWN);
                    numMovs++;
                }
                if(!this->checkCollision((*it)->position + glm::vec2(0,-1)) && (*it)->movement!=MOVE_DOWN) {
                    movsPosibles.push_back(MOVE_UP);
                    numMovs++;
                }
                if (numMovs>0) {
                    (*it)->state = MOVING;
                    (*it)->movement = movsPosibles[rand() % numMovs];
                    switch((*it)->movement) {
                        case MOVE_UP: (*it)->setDestination((*it)->getPosition() + glm::vec2(0,-1));
                        break;
                        case MOVE_DOWN: (*it)->setDestination((*it)->getPosition() + glm::vec2(0,1));
                        break;
                        case MOVE_LEFT: (*it)->setDestination((*it)->getPosition() + glm::vec2(-1,0));
                        break;
                        case MOVE_RIGHT: (*it)->setDestination((*it)->getPosition() + glm::vec2(1,0));
                        break;
                    }
                } else {
                    numMovs = 0;
                    if(!this->checkCollision((*it)->position + glm::vec2(1,0))) {
                        movsPosibles.push_back(MOVE_RIGHT);
                        numMovs++;
                    }
                    if(!this->checkCollision((*it)->position + glm::vec2(-1,0))) {
                        movsPosibles.push_back(MOVE_LEFT);
                        numMovs++;
                    }
                    if(!this->checkCollision((*it)->position + glm::vec2(0,1))) {
                        movsPosibles.push_back(MOVE_DOWN);
                        numMovs++;
                    }
                    if(!this->checkCollision((*it)->position + glm::vec2(0,-1))) {
                        movsPosibles.push_back(MOVE_UP);
                        numMovs++;
                    }
                    (*it)->movement = movsPosibles[rand() % numMovs];
                }

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

            if ((*it)->state == MOVING) {
                (*it)->setFrameHandler((*it)->getFrameHandler() + interpolation);
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
                    frame->setIndex(frame->getIndexOrig() + glm::vec2(orientation*2 + (*it)->getFrameIndex(),0));
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

        while(liveEnemies < 3 && deadEnemies<=3) {
            Iceblock* eggblock = eggBlocks.back();
            eggBlocks.pop_back();
            eggblock->disintegrate(this, false);
            // Create SnoBee Egg
            SnobeeEgg* egg = new SnobeeEgg(eggblock->getPosition(), glm::vec2(1,1), 0.07f, eggsTexture, GREEN);//glm::vec2(0.5f, 2.0f)
            egg->configureFrame(160, 160, glm::vec2(0,4));
            this->eggs.push_back(egg);
            state = LEVEL_SHOWING_EGGS;

            liveEnemies++;
        }
        if (deadEnemies == 6) {
            // WIN LEVEL
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
            ResourceManager::soundEngine->stopAllSounds();
            ResourceManager::soundEngine->play2D("sounds/diamond-blocks-lined-up.wav", true);
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
        }
        if (bonusOffset==50) {
            ResourceManager::soundEngine->stopAllSounds();
            ResourceManager::soundEngine->play2D("sounds/counting-bonus-points.wav", true);
        }
        if (bonusOffset>50) {
            Game::score += 50;
        }
        if (Game::score >= scoreObj) {//bonusOffset>150
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

            bonusOffset = 0;
            ResourceManager::soundEngine->stopAllSounds();
            ResourceManager::soundEngine->play2D("sounds/level.wav", true);
            state = LEVEL_PLAY;
        }
    }

    if (state == LEVEL_LOSE) {
        countLose++;
        if (countLose>=50) {
            countLose = 0;
            state = LEVEL_LOSE2;
            ResourceManager::soundEngine->play2D("sounds/miss.wav", false);
        }
    }
    if (state == LEVEL_LOSE2) {
        countLose++;
        if (!ResourceManager::soundEngine->isCurrentlyPlaying("sounds/miss.wav")) {
            state = LEVEL_TMP;
            countLose = 0;
        } else {
            if (countLose%4 == 0){
                pengo->changeIndexFrame(glm::vec2(((GLint) pengo->getSpriteFrame()->getIndex().x + 1) % 2, 2));
            }
        }
    }
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

    fieldStart.clear();
    for(auto &i : field) {
        for (auto &j : i) {
            delete j;
        }
        i.clear();
    }
}
