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


GLboolean Snobee::nextMove(GameLevel* level, GLboolean comeBack) {
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

Snobee::~Snobee() {

}

void Snobee::numb(GLboolean isNumb) {
	this->isNumb = isNumb;
}