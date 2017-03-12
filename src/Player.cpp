

#include "Player.h"
#include "ResourceManager.h"

GLuint indexTex = 0;
GLfloat n = 0;

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

void Player::move(Move move, GLfloat interpolation) {
    glm::vec2 diff = position-destination;
    float dist = sqrt((diff.x*diff.x) + (diff.y*diff.y));
    float sp = this->velocity*interpolation;
    float velocity = sp<dist ? sp : dist;
    //if (isMoving && (diff.x*diff.x > velocity*velocity || diff.y*diff.y > velocity*velocity)) {
    if (position != destination) {
        switch(move) {
            case MOVE_UP: this->position += glm::vec2(0, -velocity);
            break;
            case MOVE_DOWN: this->position += glm::vec2(0, velocity);
            break;
            case MOVE_LEFT: this->position += glm::vec2(-velocity, 0);
            break;
            case MOVE_RIGHT: this->position += glm::vec2(velocity, 0);
            break;
        }
        n+=interpolation;
        if (n > 4) {
            n = 0;
            indexTex = (indexTex+1) % 2;
        }
    }
    if (position == destination) {
        isMoving = false;
    }
}

void Player::setSprite(Move move) {
    switch(move) {
        case MOVE_UP: 
            this->sprite = moveUpTextures[indexTex];
            lastMove = move;
        break;
        case MOVE_DOWN: 
            this->sprite = moveDownTextures[indexTex];
            lastMove = move;
        break;
        case MOVE_LEFT: 
            this->sprite = moveLeftTextures[indexTex];
            lastMove = move;
        break;
        case MOVE_RIGHT: 
            this->sprite = moveRightTextures[indexTex];
            lastMove = move;
        break;
    }
}

