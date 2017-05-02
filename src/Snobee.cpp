#include "Snobee.h"
#include <stdlib.h>

Snobee::Snobee(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type)
    : GameObject(pos, size, velocity, initialSprite, false, SHAPE_CIRCLE_SMALL), type(type), isMoving(false), isNumb(0), lastPosition(pos)
{
    /* initialize random seed: */
    srand (time(NULL));

    Component3D* snobee3D = new Component3D(ResourceManager::getMesh("snobee"), false);
    GLfloat scaleSnobee = 12*(Game::windowHeight / 18.0f)/MAP_SCALE;
    snobee3D->setPosition(glm::vec3(pos.x,-0.5,pos.y) * (Game::windowHeight / 18.0f));
    snobee3D->setScale(glm::vec3(-1,1,1) * scaleSnobee);

    Component3D* snobeeArm1L = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm1L->setPosition(glm::vec3(-0.816,0,-0.044)); // Relative to Snobee
    snobeeArm1L->setParent(snobee3D);// Child 0

    Component3D* snobeeArm1R = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm1R->setPosition(glm::vec3(0.816,0,-0.044)); // Relative to Snobee
    snobeeArm1R->setScale(glm::vec3(-1,1,1));
    snobeeArm1R->setParent(snobee3D);// Child 1

    Component3D* snobeeArm2L = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm2L->setPosition(glm::vec3(-0.7356,0,0)); // Relative to SnobeeArm1L
    snobeeArm2L->setRotation(glm::vec3(0,90,0)); // Relative to SnobeeArm1L
    snobeeArm2L->setParent(snobeeArm1L);// Child 0

    Component3D* snobeeArm2R = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm2R->setPosition(glm::vec3(-0.7356,0,0)); // Relative to SnobeeArm1R
    snobeeArm2R->setRotation(glm::vec3(0,90,0)); // Relative to SnobeeArm1R
    snobeeArm2R->setParent(snobeeArm1R);// Child 0


    Component3D* snobeeHandL = new Component3D(ResourceManager::getMesh("snobeeHand"));
    snobeeHandL->setPosition(glm::vec3(-1.12,0,0)); // Relative to SnobeeArmL
    snobeeHandL->setParent(snobeeArm2L);// Child 0

    Component3D* snobeeHandR = new Component3D(ResourceManager::getMesh("snobeeHand"));
    snobeeHandR->setPosition(glm::vec3(-1.12,0,0)); // Relative to SnobeeArmL
    snobeeHandR->setParent(snobeeArm2R);// Child 0

    setComp3D(snobee3D);
    drawChilds = false;
}

/*
 * Returns Euclidean distance between pos1 and pos2
 */
GLfloat euclideanDistance(glm::vec2 pos1, glm::vec2 pos2) {
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

/*
 * Returns Manhattan distance between pos1 and pos2
 */
GLfloat manhattanDistance(glm::vec2 pos1, glm::vec2 pos2) {
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

/**
 * Check if there is a Snobee at position 'pos' in the map.
 */
GLboolean checkSnobees(GameLevel* level, glm::vec2 pos) {
    for (Snobee* e : level->enemies) {
        if (e != nullptr && (euclideanDistance(e->position, pos) < 1.0f)) {
            return true;
        }
    }
    return false;
}

GLboolean Snobee::nextMoveRandom(GameLevel* level, GLboolean comeBack) {
    // Next random pos
    int numMovs = 0;
    std::vector< Move > movsPosibles;

    // Check position UP
    glm::vec2 positionToCheck = position + glm::vec2(0,-1);
    if(!level->checkWalls(positionToCheck)
       && !level->checkEggAndDiamondBlocks(positionToCheck)
       && !checkSnobees(level, positionToCheck)) {
        movsPosibles.push_back(MOVE_UP);
        numMovs++;
    }

    // Check position RIGHT
    positionToCheck = position + glm::vec2(1,0);
    if(!level->checkWalls(positionToCheck)
       && !level->checkEggAndDiamondBlocks(positionToCheck)
       && !checkSnobees(level, positionToCheck)) {
        movsPosibles.push_back(MOVE_RIGHT);
        numMovs++;
    }

    // Check position DOWN
    positionToCheck = position + glm::vec2(0,1);
    if(!level->checkWalls(positionToCheck)
       && !level->checkEggAndDiamondBlocks(positionToCheck)
       && !checkSnobees(level, positionToCheck)) {
        movsPosibles.push_back(MOVE_DOWN);
        numMovs++;
    }

    // Check position LEFT
    positionToCheck = position + glm::vec2(-1,0);
    if(!level->checkWalls(positionToCheck)
       && !level->checkEggAndDiamondBlocks(positionToCheck)
       && !checkSnobees(level, positionToCheck)) {
        movsPosibles.push_back(MOVE_LEFT);
        numMovs++;
    }

    if (numMovs>0) {
        state = MOVING;
        movement = movsPosibles[rand() % numMovs];
        switch(movement) {
            case MOVE_UP: {
                setDestination(getPosition() + glm::vec2(0,-1));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr && block->state == STOPPED) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
            case MOVE_DOWN: {
                setDestination(getPosition() + glm::vec2(0,1));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr && block->state == STOPPED) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
            case MOVE_LEFT: {
                setDestination(getPosition() + glm::vec2(-1,0));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr && block->state == STOPPED) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
            case MOVE_RIGHT: {
                setDestination(getPosition() + glm::vec2(1,0));
                Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
                if (block != nullptr && block->state == STOPPED) {
                    state = DESTROYING;
                    block->disintegrate(level,false);
                }
            }
            break;
        }
    }
    return numMovs;
}

void Snobee::nextMovePursuit(GameLevel* level, GLboolean (&positionsTaken)[4]) {
    GLfloat minDistance = 999;
    Move moveToDo = MOVE_NONE;
    glm::vec2 positionToGo;

    if (manhattanDistance(this->position, level->pengo->position) <= 2 || level->liveEnemies == 1) {
        positionToGo = level->pengo->position;
    }
    else {
        // Ir por el norte de PENGO
        if (manhattanDistance(this->position, level->pengo->position + glm::vec2(0,-2)) < minDistance && !positionsTaken[0]) {
            positionToGo = level->pengo->position + glm::vec2(0,-2);
            minDistance = manhattanDistance(this->position, level->pengo->position + glm::vec2(0,-2));
            positionsTaken[0] = true;
        }
        // Ir por el sur de PENGO
        if (manhattanDistance(this->position, level->pengo->position + glm::vec2(0,2)) < minDistance && !positionsTaken[0]) {
            positionToGo = level->pengo->position + glm::vec2(0,2);
            minDistance = manhattanDistance(this->position, level->pengo->position + glm::vec2(0,2));
            positionsTaken[1] = true;
        }
        // Ir por el este de PENGO
        if (manhattanDistance(this->position, level->pengo->position + glm::vec2(2,0)) < minDistance && !positionsTaken[2]) {
            positionToGo = level->pengo->position + glm::vec2(2,0);
            minDistance = manhattanDistance(this->position, level->pengo->position + glm::vec2(2,0));
            positionsTaken[2] = true;
        }
        // Ir por el oeste de PENGO
        if (manhattanDistance(this->position, level->pengo->position + glm::vec2(-2,0)) < minDistance && !positionsTaken[3]) {
            positionToGo = level->pengo->position + glm::vec2(-2,0);
            minDistance = manhattanDistance(this->position, level->pengo->position + glm::vec2(-2,0));
            positionsTaken[3] = true;
        }
    }
    minDistance = 999;
    GLfloat distanceToDestination;

    glm::vec2 positionToCheck = position + glm::vec2(1,0);
    if(!level->checkWalls(positionToCheck)) {
        int j = position.x+1 - 0.5f;
        int i = position.y - 2;
        Iceblock* block = dynamic_cast<Iceblock*>(level->field[i][j]);
        if(level->field[i][j]==nullptr || (block!=nullptr && !block->isEggBlock)){
            distanceToDestination = manhattanDistance(positionToCheck, positionToGo);
            if (distanceToDestination <= minDistance) {
                minDistance = distanceToDestination;
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
            distanceToDestination = manhattanDistance(positionToCheck, positionToGo);
            if (distanceToDestination <= minDistance) {
                minDistance = distanceToDestination;
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
            distanceToDestination = manhattanDistance(positionToCheck, positionToGo);
            if (distanceToDestination <= minDistance) {
                minDistance = distanceToDestination;
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
            distanceToDestination = manhattanDistance(positionToCheck, positionToGo);
            if (distanceToDestination <= minDistance) {
                minDistance = distanceToDestination;
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
            if (block != nullptr && block->state == STOPPED) {
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
            if (block != nullptr && block->state == STOPPED) {
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
            if (block != nullptr && block->state == STOPPED) {
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
            if (block != nullptr && block->state == STOPPED) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        break;
    }
}

void Snobee::nextMoveANN(GameLevel* level) {

    std::vector<float> input(4);
    // Check position UP
    glm::vec2 positionToCheck = position + glm::vec2(0,-1);
    if(level->checkWalls(positionToCheck)
       || level->checkEggAndDiamondBlocks(positionToCheck)
       || checkSnobees(level, positionToCheck)) {
           input[0] = -1;
    }
    else {
        input[0] = euclideanDistance(positionToCheck, level->pengo->position);
        Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(positionToCheck));
        if (block != nullptr || (positionToCheck == this->lastPosition)) {
            input[0] += 2;
        }
    }

    // Check position RIGHT
    positionToCheck = position + glm::vec2(1,0);
    if(level->checkWalls(positionToCheck)
       || level->checkEggAndDiamondBlocks(positionToCheck)
       || checkSnobees(level, positionToCheck)) {
           input[1] = -1;
    }
    else {
        input[1] = euclideanDistance(positionToCheck, level->pengo->position);
        Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(positionToCheck));
        if (block != nullptr || (positionToCheck == this->lastPosition)) {
            input[1] += 2;
        }
    }

    // Check position DOWN
    positionToCheck = position + glm::vec2(0,1);
    if(level->checkWalls(positionToCheck)
       || level->checkEggAndDiamondBlocks(positionToCheck)
       || checkSnobees(level, positionToCheck)) {
           input[2] = -1;
    }
    else {
        input[2] = euclideanDistance(positionToCheck, level->pengo->position);
        Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(positionToCheck));
        if (block != nullptr || (positionToCheck == this->lastPosition)) {
            input[2] += 2;
        }
    }

    // Check position LEFT
    positionToCheck = position + glm::vec2(-1,0);
    if(level->checkWalls(positionToCheck)
       || level->checkEggAndDiamondBlocks(positionToCheck)
       || checkSnobees(level, positionToCheck)) {
           input[3] = -1;
    }
    else {
        input[3] = euclideanDistance(positionToCheck, level->pengo->position);
        Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(positionToCheck));
        if (block != nullptr || (positionToCheck == this->lastPosition)) {
            input[3] += 2;
        }
    }

    // Calculate movement
    std::vector<float> output = Game::mlp->recallNetwork(input);

    // Move UP
    if (output[0] < 0.5f && output[1] < 0.5f) {
        if(input[0] >= 0.0f) {
            movement = MOVE_UP;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(0,-1));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr && block->state == STOPPED) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        else {
            this->nextMoveRandom(level, true);
        }
    }
    // Move RIGHT
    else if (output[0] < 0.5f && output[1] >= 0.5f) {
        if(input[1] >= 0.0f) {
            movement = MOVE_RIGHT;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(1,0));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr && block->state == STOPPED) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        else {
            this->nextMoveRandom(level, true);
        }
    }
    // Move DOWN
    else if (output[0] >= 0.5f && output[1] < 0.5f) {
        if(input[2] >= 0.0f) {
            movement = MOVE_DOWN;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(0,1));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr && block->state == STOPPED) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        else {
            this->nextMoveRandom(level, true);
        }
    }
    // Move LEFT
    else if (output[0] >= 0.5f && output[1] >= 0.5f) {
        if(input[3] >= 0.0f) {
            movement = MOVE_LEFT;
            state = MOVING;
            setDestination(getPosition() + glm::vec2(-1,0));
            Iceblock* block = dynamic_cast<Iceblock*>(level->getObjFromPosition(getDestination()));
            if (block != nullptr && block->state == STOPPED) {
                state = DESTROYING;
                block->disintegrate(level,false);
            }
        }
        else {
            this->nextMoveRandom(level, true);
        }
    }
    this->lastPosition = this->position;
}

Snobee::~Snobee() {

}

void Snobee::numb(GLboolean isNumb) {
	this->isNumb = isNumb;
}

void Snobee::update() {
    GLfloat rotSin2 = glm::sin(frame3D/2);
    GLfloat moveJump = 0;
    GLfloat shear = 0;

    if (state == MOVING){
        drawChilds = false;
        //component3D->setScale(glm::vec3(-1,rotSin2*0.2 + 0.8,1) * 12.0f);
        moveJump = glm::sin(frame3D/2-90)*0.2;
        frame3D++;
    } else if(state == DESTROYING) {
        drawChilds = true;
        frame3D++;
    } else if(state == NUMB) {
        shear = rotSin2;
        frame3D++;
    }

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

    component3D->setRotation(glm::vec3(0,orientation * 90 + 180,0));
    component3D->setPosition(glm::vec3(position.x, 0.5 + moveJump, position.y) * (Game::windowHeight / 18.0f));
    component3D->setShear(shear/2);

    component3D->childs[0]->setRotation(glm::vec3(0.0f,-rotSin2*70,0.0f));
    component3D->childs[1]->setRotation(glm::vec3(0.0f,rotSin2*70,0.0f));
    component3D->childs[1]->childs[0]->setRotation(glm::vec3(0,-90 -rotSin2*70,0));
    component3D->childs[0]->childs[0]->setRotation(glm::vec3(0,-90 +rotSin2*70,0));
}
