

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

void Player::move(PlayerMove move) {

    if (position != destination) {
        switch(move) {
            case MOVE_UP: this->position += glm::vec2(0, -this->velocity);
            break;
            case MOVE_DOWN: this->position += glm::vec2(0, this->velocity);
            break;
            case MOVE_LEFT: this->position += glm::vec2(-this->velocity, 0);
            break;
            case MOVE_RIGHT: this->position += glm::vec2(this->velocity, 0);
            break;
        }
        if (++n == 4) {
            n = 0;
            indexTex = (indexTex+1) % 2;
        }
    } else {
        isMoving = false;
    }
}

void Player::setSprite(PlayerMove move) {
    switch(move) {
        case MOVE_UP: this->sprite = moveUpTextures[indexTex];
        break;
        case MOVE_DOWN: this->sprite = moveDownTextures[indexTex];
        break;
        case MOVE_LEFT: this->sprite = moveLeftTextures[indexTex];
        break;
        case MOVE_RIGHT: this->sprite = moveRightTextures[indexTex];
        break;
    }
}

