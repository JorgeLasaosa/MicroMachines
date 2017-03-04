
#include "GameLevel.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#define nullNode glm::vec2(-1,-1)

GameLevel::GameLevel():field(15, std::vector<GameObject*>(13)),fieldStart(15, std::vector<GameObject*>(13)) {
    genNodeActual = nullNode;
}


GameLevel::~GameLevel() {
    delete pengo;
}

void GameLevel::load(const GLchar* filePath) {
    //Clear old data

    int xOffset = 100, yOffset = 40;
    // Pengo
    Texture pengo0Texture = ResourceManager::getTexture("pengoDown0");
    this->pengo = new Player(glm::vec2(xOffset+20+40*6,yOffset+20+40*6), glm::vec2(40,40), 5.0f, pengo0Texture);

    // Walls
    Texture wall0Texture = ResourceManager::getTexture("wall0");
    Texture wall1Texture = ResourceManager::getTexture("wall1");

    for (int i=0; i<28; i++) {
        wallN.push_back(Wallblock(glm::vec2(xOffset + 20 * i, yOffset), glm::vec2(20,20), wall1Texture));
        wallS.push_back(Wallblock(glm::vec2(xOffset + 20 * i, yOffset + 620), glm::vec2(20,20), wall1Texture));
    }

    for (int i=0; i<30; i++) {
        wallE.push_back(Wallblock(glm::vec2(xOffset + 540, yOffset + 20 + 20 * i), glm::vec2(20,20), wall0Texture));
        wallW.push_back(Wallblock(glm::vec2(xOffset, yOffset + 20 + 20 * i), glm::vec2(20,20), wall0Texture));
    }

    // Blocks
    Texture iceblockTexture = ResourceManager::getTexture("iceblock");
    Texture diamondTexture = ResourceManager::getTexture("diamond");
    std::ifstream file(filePath);
    std::string line;
    GLuint n;
    int i = 0, j = 0;
    if (file) {
        while(std::getline(file, line)) {
            std::istringstream iss(line);
            while(iss >> n) {
                if (n == 1) {
                    field[i][j] = new Iceblock(glm::vec2(xOffset+20+40*j, yOffset+20+40*i), glm::vec2(40,40), 10.0f, iceblockTexture);
                }
                else if (n == 2) {
                    field[i][j] = new Diamondblock(glm::vec2(xOffset+20+40*j, yOffset+20+40*i), glm::vec2(40,40), 10.0f, diamondTexture);
                } else {
                    fieldStart[i][j] = new Iceblock(glm::vec2(xOffset+20+40*j, yOffset+20+40*i), glm::vec2(40,40), 10.0f, iceblockTexture);
                }
                j++;
            }
            j=0;
            i++;
        }
    }
}

bool GameLevel::generate() {
    bool end = false;
    if (genNodeActual==nullNode && mazeNodesStart.size() == 0) {
        end = true;
        // Buscar siguiente de abajo a arriba e izda a dcha
        for(int i = 14; i >=0 && end; i--) {
            for(int j = 0; j < 12 && end; j++) {
                if (fieldStart[i][j]!=nullptr) {
                    end = false;
                    mazeNodesStart.push(glm::vec2(i,j));
                }
            }
        }
    }
    if(!end) {//
        if(genNodeActual==nullNode){
            genNodeActual = mazeNodesStart.front();
            mazeNodesStart.pop();
        }
        int n_x = genNodeActual.x, n_y = genNodeActual.y;
        fieldStart[n_x][n_y] = nullptr;
        genNodeActual = nullNode;

        int numVecinos = 0;

        // Comprobar arriba
        if(n_y>0 && fieldStart[n_x][n_y-1] != nullptr) {
            genNodeActual = glm::vec2(n_x,n_y-1);
            numVecinos++;
        }

        // Comprobar izquierda
        if(n_x>0 && fieldStart[n_x-1][n_y] != nullptr) {
            if (numVecinos==0) {
                genNodeActual = glm::vec2(n_x-1,n_y);
            } else {
                mazeNodesStart.push(glm::vec2(n_x-1,n_y));
            }
            numVecinos++;
        }

        // Comprobar derecha
        if(n_x<14 && fieldStart[n_x+1][n_y] != nullptr) {
            if (numVecinos==0) {
                genNodeActual = glm::vec2(n_x+1,n_y);
            } else {
                mazeNodesStart.push(glm::vec2(n_x+1,n_y));
            }
            numVecinos++;
        }

        // Comprobar abajo
        if(n_y<12 && fieldStart[n_x][n_y+1] != nullptr) {
            if (numVecinos==0) {
                genNodeActual = glm::vec2(n_x,n_y+1);
            } else {
                mazeNodesStart.push(glm::vec2(n_x,n_y+1));
            }
            numVecinos++;
        }
    }
    return end;
}

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
}

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


bool GameLevel::checkCollision(glm::vec2 pos){
    int xOffset = 100, yOffset = 40;
    int j = (pos.x - xOffset+20)/40 - 1;
    int i = (pos.y - yOffset+20)/40 - 1;
    if (i>=15 || i < 0 || j>=13 || j < 0) return true;
    return field[i][j]!=nullptr;
}