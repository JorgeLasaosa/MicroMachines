#include "GameObject.h"

GameObject::GameObject()
    : position(0,0), size(1,1), velocity(0.0f), sprite(), isPushable(false),  state(STOPPED),lastState(STOPPED), frameHandler(0), 
    frameIndex(0),lastDist(-1), hasComp3D(false),frame3D(0) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite, GLboolean isPushable, Shape shape)
    : position(pos), size(size), velocity(velocity),sprite(sprite), isPushable(isPushable), state(STOPPED), lastState(STOPPED), 
    frameHandler(0), frameIndex(0), shape(shape), killing(0), origPos(pos), lastDist(-1), hasComp3D(false),frame3D(0),drawChilds(true) {}

GameObject::~GameObject() {
	//dtor
}

void GameObject::configureFrame(GLint frame_width, GLint frame_height, glm::vec2 index) {
    this->frame = SpriteFrame(this->sprite.WIDTH, this->sprite.HEIGHT, frame_width, frame_height, index);
}
void GameObject::changeIndexFrame(glm::vec2 index){
    this->frame.setIndex(index);
}

void GameObject::setComp3D(Component3D* component3D){
    hasComp3D = true;
    this->component3D = component3D;
}

void GameObject::draw(SpriteRenderer& renderer) {
    if (Game::mode3D && hasComp3D){
        component3D->draw(drawChilds);
    } else {
        renderer.drawSprite(this->sprite, this->position, this->size, this->frame);
    }
}

void GameObject::draw(SpriteRenderer& renderer, GLfloat interpolation) {
    if (Game::mode3D && hasComp3D){
        component3D->draw(drawChilds);
    } else {
        renderer.drawSprite(this->sprite, this->position + (this->velocity * interpolation), this->size, this->frame);
    }
}

bool GameObject::move(GLfloat interpolation) {
    GLfloat realVelocity = this->velocity;
    if (state == DESTROYING) {
        realVelocity = realVelocity/2;
    }
    glm::vec2 diff = position-destination;
    GLfloat dist = sqrt((diff.x*diff.x) + (diff.y*diff.y));
    GLfloat sp = realVelocity*interpolation;
    GLfloat velocity = sp<dist ? sp : dist;// min
    if (position != destination) {
        switch(movement) {
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


    if (position == destination || (lastDist>=0 && lastDist<dist)) {
        state = STOPPED;
        lastDist = -1;
        return false;
    }

    lastDist = dist;
    return true;
}

bool GameObject::overlaps(GameObject* obj) {
    bool collision = false;
    if (shape == SHAPE_CIRCLE || shape == SHAPE_CIRCLE_SMALL) {
        GLfloat radius1 = size.x/2;
        if (shape == SHAPE_CIRCLE_SMALL) radius1 = radius1/2;
        if (obj->getShape() == SHAPE_CIRCLE || obj->getShape() == SHAPE_CIRCLE_SMALL){
            // Euclidean distance
            glm::vec2 objPos = obj->getPosition();
            GLfloat radius2 = obj->getSize().x/2;
        if (obj->getShape() == SHAPE_CIRCLE_SMALL) radius2 = radius2/2;
            GLfloat distance = radius1 + radius2;
            if (sqrt(pow(objPos.x - position.x,2) + pow(objPos.y - position.y,2)) < distance) {
                collision = true;
            }
        } else if (obj->getShape() == SHAPE_RECTANGLE){
            glm::vec2 objPos = obj->getPosition();
            glm::vec2 objSize = obj->getSize();
            GLfloat centerX = position.x + size.x/2;
            GLfloat centerY = position.y + size.y/2;

            if( centerX>objPos.x && centerX < objPos.x + objSize.x) {
                // Distance to hor. edges
                if((objPos.y - centerY >= radius1) != (centerY - (objPos.y + objSize.y) < radius1)) {
                    collision = true;
                }
            } else if( centerY>objPos.y && centerY < objPos.y + objSize.y) {
                // Distance to ver. edges
                if((objPos.x - centerX >= radius1) != (centerX - (objPos.x + objSize.x) < radius1)) {
                    collision = true;
                }
            } else if (sqrt(pow(centerX - position.x,2) + pow(centerY - position.y,2)) < radius1
                || sqrt(pow(centerX - (position.x + objSize.x),2) + pow(centerY - position.y,2)) < radius1
                || sqrt(pow(centerX - position.x,2) + pow(centerY - (position.y + objSize.y),2)) < radius1
                || sqrt(pow(centerX - (position.x + objSize.x),2) + pow(centerY - (position.y + objSize.y),2)) < radius1) {
                // Distance to corners
                collision = true;
            }
        } else if (obj->getShape() == SHAPE_DOT){
            glm::vec2 objPos = obj->getPosition();
            GLfloat distance = size.x/2;// Radius + radius
            if (sqrt(pow(objPos.x - position.x,2) + pow(objPos.y - position.y,2)) < distance) {
                collision = true;
            }
        }
    } else if (shape == SHAPE_RECTANGLE) {
        if (obj->getShape() == SHAPE_RECTANGLE) {
            glm::vec2 objPos = obj->getPosition();
            glm::vec2 objSize = obj->getSize();
            if (((objPos.x < position.x + size.x && objPos.x >= position.x) || (position.x < objPos.x + objSize.x && position.x >= objPos.x)) && 
                ((objPos.y < position.y + size.y && objPos.y >= position.y) || (position.y < objPos.y + objSize.y && position.y >= objPos.y))) {
                collision = true;
            }
        } else if(obj->getShape() == SHAPE_CIRCLE || obj->getShape() == SHAPE_CIRCLE_SMALL) {
            glm::vec2 objPos = obj->getPosition();
            glm::vec2 objSize = obj->getSize();
            GLfloat centerX = objPos.x + objSize.x/2;
            GLfloat centerY = objPos.y + objSize.y/2;
            GLfloat radius1 = objSize.x/2;
            if (obj->getShape() == SHAPE_CIRCLE_SMALL) radius1 = radius1/2;

            if( centerX>position.x && centerX < position.x + size.x) {
                // Distance to hor. edges
                if((position.y - centerY >= radius1) != (centerY - (position.y + size.y) < radius1)) {
                    collision = true;
                }
            } else if( centerY>position.y && centerY < position.y + size.y) {
                // Distance to ver. edges
                if((position.x - centerX >= radius1) != (centerX - (position.x + size.x) < radius1)) {
                    collision = true;
                }
            } else if (sqrt(pow(centerX - objPos.x,2) + pow(centerY - objPos.y,2)) < radius1
                || sqrt(pow(centerX - (objPos.x + size.x),2) + pow(centerY - objPos.y,2)) < radius1
                || sqrt(pow(centerX - objPos.x,2) + pow(centerY - (objPos.y + size.y),2)) < radius1
                || sqrt(pow(centerX - (objPos.x + size.x),2) + pow(centerY - (objPos.y + size.y),2)) < radius1) {
                // Distance to corners
                collision = true;
            }
        } else if(obj->getShape() == SHAPE_DOT) {
            glm::vec2 objPos = obj->getPosition();
            glm::vec2 objSize = obj->getSize();
            if ((objPos.x < position.x + size.x && objPos.x >= position.x) && 
                (objPos.y < position.y + size.y && objPos.y >= position.y)) {
                collision = true;
            }
        }
    } else if (shape == SHAPE_DOT) {
        if (obj->getShape() == SHAPE_RECTANGLE) {
            glm::vec2 objPos = obj->getPosition();
            glm::vec2 objSize = obj->getSize();
            if (((position.x < objPos.x + objSize.x && position.x >= objPos.x)) && 
                ((position.y < objPos.y + objSize.y && position.y >= objPos.y))) {
                collision = true;
            }
        } else if(obj->getShape() == SHAPE_DOT) {
            // TODO
        } else if (obj->getShape() == SHAPE_CIRCLE || obj->getShape() == SHAPE_CIRCLE_SMALL){
            glm::vec2 objPos = obj->getPosition();
            GLfloat distance = obj->getSize().x/2;// Radius + radius
            if (obj->getShape() == SHAPE_CIRCLE_SMALL) distance = distance/2;
            if (sqrt(pow(objPos.x - position.x,2) + pow(objPos.y - position.y,2)) < distance) {
                collision = true;
            }
        }
    }

    return collision;
}