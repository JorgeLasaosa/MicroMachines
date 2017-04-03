#include "Snobee.h"
#include <stdlib.h>

Snobee::Snobee(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type)
    : GameObject(pos, size, velocity, initialSprite, false, SHAPE_CIRCLE_SMALL), type(type), isMoving(false), isNumb(0)
{
    /* initialize random seed: */
    srand (time(NULL));
}

//Snobee::move() {
//    if (!isMoving) {
//        switch(this->type) {
//            case GREEN:
//                break;
//            case YELLOW:
//                break;
//            case RED:
//                break;
//        }
//    }
//}

GLboolean Snobee::nextMoveRandom(GameLevel* level, GLboolean comeBack) {
    // Next random pos
    int numMovs = 0;
    std::vector< Move > movsPosibles;
    if(!level->checkWalls(position + glm::vec2(1,0)) && (movement!=MOVE_LEFT || comeBack)) {
        int j = position.x+1 - 0.5f;
        int i = position.y - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            movsPosibles.push_back(MOVE_RIGHT);
            numMovs++;
        }
    }
    if(!level->checkWalls(position + glm::vec2(-1,0)) && (movement!=MOVE_RIGHT || comeBack)) {
        int j = position.x-1 - 0.5f;
        int i = position.y - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            movsPosibles.push_back(MOVE_LEFT);
            numMovs++;
        }
    }
    if(!level->checkWalls(position + glm::vec2(0,1)) && (movement!=MOVE_UP || comeBack)) {
        int j = position.x - 0.5f;
        int i = position.y+1 - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            movsPosibles.push_back(MOVE_DOWN);
            numMovs++;
        }
    }
    if(!level->checkWalls(position + glm::vec2(0,-1)) && (movement!=MOVE_DOWN || comeBack)) {
        int j = position.x - 0.5f;
        int i = position.y-1 - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            movsPosibles.push_back(MOVE_UP);
            numMovs++;
        }
    }
    if (numMovs>0) {
        state = MOVING;
        movement = movsPosibles[rand() % numMovs];
        switch(movement) {
            case MOVE_UP: {
                setDestination(getPosition() + glm::vec2(0,-1));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
            case MOVE_DOWN: {
                setDestination(getPosition() + glm::vec2(0,1));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
            case MOVE_LEFT: {
                setDestination(getPosition() + glm::vec2(-1,0));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
            case MOVE_RIGHT: {
                setDestination(getPosition() + glm::vec2(1,0));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
        }
    }
    return numMovs;
}

/*
 * Returns Manhattan distance between pos1 and pos2
 */
GLfloat manhattanDistance(glm::vec2 pos1, glm::vec2 pos2) {
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

void Snobee::nextMovePursuit(GameLevel* level) {
    GLfloat minDistance = 999;
    GLfloat distanceToPengo;
    Move moveToDo = MOVE_NONE;

    glm::vec2 positionToCheck = position + glm::vec2(1,0);
    if(!level->checkWalls(positionToCheck)) {
        int j = position.x+1 - 0.5f;
        int i = position.y - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            distanceToPengo = manhattanDistance(positionToCheck, level->pengo->position);
            if (distanceToPengo <= minDistance) {
                minDistance = distanceToPengo;
                moveToDo = MOVE_RIGHT;
            }
        }
    }
    positionToCheck = position + glm::vec2(-1,0);
    if(!level->checkWalls(positionToCheck)) {
        int j = position.x-1 - 0.5f;
        int i = position.y - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            distanceToPengo = manhattanDistance(positionToCheck, level->pengo->position);
            if (distanceToPengo <= minDistance) {
                minDistance = distanceToPengo;
                moveToDo = MOVE_LEFT;
            }
        }
    }
    positionToCheck = position + glm::vec2(0,1);
    if(!level->checkWalls(positionToCheck)) {
        int j = position.x - 0.5f;
        int i = position.y+1 - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            distanceToPengo = manhattanDistance(positionToCheck, level->pengo->position);
            if (distanceToPengo <= minDistance) {
                minDistance = distanceToPengo;
                moveToDo = MOVE_DOWN;
            }
        }
    }
    positionToCheck = position + glm::vec2(0,-1);
    if(!level->checkWalls(positionToCheck)) {
        int j = position.x - 0.5f;
        int i = position.y-1 - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            distanceToPengo = manhattanDistance(positionToCheck, level->pengo->position);
            if (distanceToPengo <= minDistance) {
                minDistance = distanceToPengo;
                moveToDo = MOVE_UP;
            }
        }
    }
    switch(moveToDo) {
        case MOVE_UP: {
            movement = MOVE_UP;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(0,-1));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        break;
        case MOVE_DOWN: {
            movement = MOVE_DOWN;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(0,1));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        break;
        case MOVE_LEFT: {
            movement = MOVE_LEFT;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(-1,0));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        break;
        case MOVE_RIGHT: {
            movement = MOVE_RIGHT;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(1,0));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        break;
    }
}

Snobee::~Snobee() {

}

void Snobee::numb(GLboolean isNumb) {
	this->isNumb = isNumb;
}
