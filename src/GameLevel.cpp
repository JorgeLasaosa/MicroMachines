#include "GameLevel.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Texture.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

GameLevel::GameLevel() {}

GameLevel::~GameLevel() {
}

void GameLevel::load(const GLchar* filePath) {
    //Clear old data
    //delete pengo;
    blocks.clear();

    int xOffset = 100, yOffset = 40;
    // Pengo
    Texture pengo0Texture = ResourceManager::getTexture("pengoDown0");
    this->pengo = new Player(glm::vec2(xOffset+20+40*6,yOffset+20+40*6), glm::vec2(40,40), pengo0Texture);

    // Walls
    Texture wall0Texture = ResourceManager::getTexture("wall0");
    Texture wall1Texture = ResourceManager::getTexture("wall1");

    for (int i=0; i<28; i++) {
        wallN.push_back(GameObject(glm::vec2(xOffset + 20 * i, yOffset), glm::vec2(20,20), wall1Texture));
        wallS.push_back(GameObject(glm::vec2(xOffset + 20 * i, yOffset + 620), glm::vec2(20,20), wall1Texture));
    }

    for (int i=0; i<30; i++) {
        wallE.push_back(GameObject(glm::vec2(xOffset + 540, yOffset + 20 + 20 * i), glm::vec2(20,20), wall0Texture));
        wallW.push_back(GameObject(glm::vec2(xOffset, yOffset + 20 + 20 * i), glm::vec2(20,20), wall0Texture));
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
                    blocks.push_back(GameObject(glm::vec2(xOffset+20+40*j, yOffset+20+40*i), glm::vec2(40,40), iceblockTexture));
                }
                else if (n == 2) {
                    blocks.push_back(GameObject(glm::vec2(xOffset+20+40*j, yOffset+20+40*i), glm::vec2(40,40), diamondTexture));
                }
                j++;
            }
            j=0;
            i++;
        }
    }
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

    for (GameObject& block : this->blocks) {
        block.draw(renderer);
    }
}
