#include "Player.h"
#include "ResourceManager.h"

GLuint indexTex = 0;
GLfloat n = 0;

Player::Player(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, GLboolean isPushable)
	: GameObject(pos, size, velocity, initialSprite, isPushable, SHAPE_CIRCLE_SMALL), destination(pos)
{
    this->movement = MOVE_DOWN;
    Component3D* pengo3D = new Component3D(ResourceManager::getMesh("pengo"), false);
    GLfloat scalePengo = 10;
    pengo3D->setPosition(glm::vec3(pos.x,0,pos.y) * MAP_SCALE);
    pengo3D->setScale(glm::vec3(1,1,1) * scalePengo);

    Component3D* pengoArmL = new Component3D(ResourceManager::getMesh("pengoArm"));
    pengoArmL->setPosition(glm::vec3(1.18751,3.59175,0)); // Relative to Pengo
    pengoArmL->setParent(pengo3D);// Child 0

    Component3D* pengoArmR = new Component3D(ResourceManager::getMesh("pengoArm"));
    pengoArmR->setPosition(glm::vec3(-1.18751,3.59175,0)); // Relative to Pengo
    pengoArmR->setScale(glm::vec3(-1,1,1));
    pengoArmR->setParent(pengo3D);// Child 1

    Component3D* pengoFeetL = new Component3D(ResourceManager::getMesh("pengoFeet"));
    pengoFeetL->setPosition(glm::vec3(0.62,1.45,0.34)); // Relative to Pengo
    pengoFeetL->setParent(pengo3D);// Child 2

    Component3D* pengoFeetR = new Component3D(ResourceManager::getMesh("pengoFeet"));
    pengoFeetR->setPosition(glm::vec3(-0.62,1.45,0.34)); // Relative to Pengo
    pengoFeetR->setScale(glm::vec3(-1,1,1));
    pengoFeetR->setParent(pengo3D);// Child 3

    setComp3D(pengo3D);
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
        if (state == PUSHING) frame3D = 0;
        lastState = state;
    }
    switch(state) {
        case PUSHING: {
            // Update frame
            frameHandler = frameHandler + 1;
            if (frame3D < 4){
                frame3D++;
            }
            if (frameHandler == 5) {
                frameIndex = frameIndex+1;
            }
            if (frameHandler > 10) {
                state = STOPPED;
                frame3D = 0;
            }
            actionFrame = 1;
        }
        break;
        case DESTROYING: {
            // Update frame
            frame3D++;
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
            frame3D++;
            if (frame3D > 9999999){
                frame3D = 0;
            }
        }
        break;
        case DEAD: {
            frame3D++;
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

    // Update 3D model
    GLfloat rotSin = glm::sin(frame3D/2);
    if (Game::mode3D && hasComp3D){
        component3D->setRotation(glm::vec3(0,-orientation * 90,0));
        if (state == MOVING){
            component3D->childs[0]->setRotation(glm::vec3( rotSin*70,0.0f,0.0f));
            component3D->childs[1]->setRotation(glm::vec3(-rotSin*70,0.0f,0.0f));
            component3D->childs[2]->setRotation(glm::vec3(-rotSin*30,0.0f,0.0f));
            component3D->childs[3]->setRotation(glm::vec3( rotSin*30,0.0f,0.0f));
        } else if (state == STOPPED){
            component3D->childs[0]->setRotation(glm::vec3(0.0f,0.0f,0.0f));
            component3D->childs[1]->setRotation(glm::vec3(0.0f,0.0f,0.0f));
            component3D->childs[2]->setRotation(glm::vec3(0.0f,0.0f,0.0f));
            component3D->childs[3]->setRotation(glm::vec3(0.0f,0.0f,0.0f));
        } else if (state == DESTROYING){
            component3D->setRotation(glm::vec3(15.0f,-orientation * 90,0.0f));
            component3D->childs[0]->setRotation(glm::vec3(rotSin*30-90,0.0f,0.0f));
            component3D->childs[1]->setRotation(glm::vec3(rotSin*30-90,0.0f,0.0f));
            component3D->childs[2]->setRotation(glm::vec3(30,0.0f,0.0f));
            component3D->childs[3]->setRotation(glm::vec3(30,0.0f,0.0f));
        } else if (state == PUSHING){
            component3D->setRotation(glm::vec3(15.0f,-orientation * 90,0.0f));
            component3D->childs[0]->setRotation(glm::vec3(-frame3D*30,0.0f,0.0f));
            component3D->childs[1]->setRotation(glm::vec3(-frame3D*30,0.0f,0.0f));
            component3D->childs[2]->setRotation(glm::vec3(30,0.0f,0.0f));
            component3D->childs[3]->setRotation(glm::vec3(30,0.0f,0.0f));
        } else if (state == DEAD){
            component3D->setRotation(glm::vec3(-90.0f,-orientation * 90,0.0f));
            component3D->childs[0]->setRotation(glm::vec3(rotSin*30-90,0.0f,0.0f));
            component3D->childs[1]->setRotation(glm::vec3(rotSin*30-90,0.0f,0.0f));
            component3D->childs[2]->setRotation(glm::vec3(-rotSin*30,0.0f,0.0f));
            component3D->childs[3]->setRotation(glm::vec3( rotSin*30,0.0f,0.0f));
        }
        component3D->setPosition(glm::vec3(position.x,0,position.y) * MAP_SCALE);
    }
}

