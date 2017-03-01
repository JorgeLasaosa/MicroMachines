

#include "Player.h"
#include "ResourceManager.h"

GLuint indexTex = 0;
GLuint n = 0;

Player::Player(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, GLboolean isPushable)
	: GameObject(pos, size, velocity, initialSprite, isPushable), destination(pos)
{
    moveDownTextures[0] = ResourceManager::getTexture("pengoDown0");
    moveDownTextures[1] = ResourceManager::getTexture("pengoDown1");
    moveLeftTextures[0] = ResourceManager::getTexture("pengoLeft0");
    moveLeftTextures[1] = ResourceManager::getTexture("pengoLeft1");
    moveUpTextures[0] = ResourceManager::getTexture("pengoUp0");
    moveUpTextures[1] = ResourceManager::getTexture("pengoUp1");
    moveRightTextures[0] = ResourceManager::getTexture("pengoRight0");
    moveRightTextures[1] = ResourceManager::getTexture("pengoRight1");
}

void Player::move(const std::string& move) {
    if (move == "up") {
        this->sprite = moveUpTextures[indexTex];
        this->position += glm::vec2(0, -this->velocity);
    }
    else if (move == "down") {
        this->sprite = moveDownTextures[indexTex];
        this->position += glm::vec2(0, this->velocity);
    }
    else if(move == "left") {
        this->sprite = moveLeftTextures[indexTex];
        this->position += glm::vec2(-this->velocity, 0);
    }
    else if (move == "right") {
        this->sprite = moveRightTextures[indexTex];
        this->position += glm::vec2(this->velocity, 0);
    }
    if (++n == 4) {
        n = 0;
        indexTex = (indexTex+1) % 2;
    }
}

