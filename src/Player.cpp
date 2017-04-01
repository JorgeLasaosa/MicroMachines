#include "Player.h"
#include "ResourceManager.h"

GLuint indexTex = 0;
GLfloat n = 0;

Player::Player(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, GLboolean isPushable)
	: GameObject(pos, size, velocity, initialSprite, isPushable, SHAPE_CIRCLE_SMALL), destination(pos)
{
}

void Player::move(Move move, GLfloat interpolation) {
    glm::vec2 diff = position-destination;
    float dist = sqrt((diff.x*diff.x) + (diff.y*diff.y));
    float sp = this->velocity*interpolation;
    float velocity = sp<dist ? sp : dist;
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
    }
    if (state == MOVING && position == destination) {
        state = STOPPED;
    }
}

void Player::update() {
    GLint actionFrame = 0;
    // UPDATE STATE
    if(lastState != state) {
        frameHandler = 0;
        if(state!=MOVING) frameIndex = 0;
        lastState = state;
    }
    switch(state) {
        case PUSHING: {
            // Update frame
            frameHandler = frameHandler + 1;
            if (frameHandler == 5) {
                frameIndex = frameIndex+1;
            }
            if (frameHandler > 10) {
                state = STOPPED;
            }
            actionFrame = 1;
        }
        break;
        case DESTROYING: {
            // Update frame
            frameHandler = frameHandler + 1;
            if ((GLint) frameHandler % 3 == 0) {
                frameIndex = ((GLint) frameIndex+1) % 2;
            }
            if (frameHandler > 10) {
                state = STOPPED;
            }
            actionFrame = 1;
        }
        break;
        case MOVING: {
            frameHandler = frameHandler + 1;
            if (frameHandler > 4) {
                frameHandler = 0;
                frameIndex = (frameIndex+1) % 2;
            }
        }
        break;
    }

    // UPDATE SPRITE
    GLint orientation = 0;
    switch(movement) {
        case MOVE_UP: orientation = 2;
        break;
        case MOVE_DOWN: orientation = 0;
        break;
        case MOVE_LEFT: orientation = 1;
        break;
        case MOVE_RIGHT: orientation = 3;
        break;
    }
    frame.setIndex(frame.getIndexOrig() + glm::vec2(orientation*2 + frameIndex,actionFrame));
}

