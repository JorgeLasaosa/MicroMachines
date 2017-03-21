#include "GameObject.h"

GameObject::GameObject()
    : position(0,0), size(1,1), velocity(0.0f), sprite(), isPushable(false),  state(STOPPED),lastState(STOPPED), frameHandler(0), frameIndex(0) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite, GLboolean isPushable)
    : position(pos), size(size), velocity(velocity),sprite(sprite), isPushable(isPushable), state(STOPPED), lastState(STOPPED), frameHandler(0), frameIndex(0) {}

GameObject::~GameObject() {
	//dtor
}


void GameObject::configureFrame(GLint frame_width, GLint frame_height, glm::vec2 index) {
    this->frame = SpriteFrame(this->sprite.WIDTH, this->sprite.HEIGHT, frame_width, frame_height, index);
}
void GameObject::changeIndexFrame(glm::vec2 index){
    this->frame.setIndex(index);
}

void GameObject::draw(SpriteRenderer& renderer) {
    renderer.drawSprite(this->sprite, this->position, this->size, this->frame);
}

void GameObject::draw(SpriteRenderer& renderer, GLfloat interpolation) {
    renderer.drawSprite(this->sprite, this->position + (this->velocity * interpolation), this->size, this->frame);
}

bool GameObject::move(GLfloat interpolation) {
    glm::vec2 diff = position-destination;
    float dist = sqrt((diff.x*diff.x) + (diff.y*diff.y));
    float sp = this->velocity*interpolation;
    float velocity = sp<dist ? sp : dist;// min
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

    if (position == destination) {
        state = STOPPED;
        return false;
    }
    return true;
}