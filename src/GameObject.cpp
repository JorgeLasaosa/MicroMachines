#include "GameObject.h"

GameObject::GameObject()
    : position(0,0), size(1,1), velocity(0.0f), sprite(), isPushable(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite, GLboolean isPushable)
    : position(pos), size(size), velocity(velocity),sprite(sprite), isPushable(isPushable) {}

GameObject::~GameObject() {
	//dtor
}

void GameObject::draw(SpriteRenderer& renderer) {
    renderer.drawSprite(this->sprite, this->position, this->size);
}

void GameObject::draw(SpriteRenderer& renderer, GLfloat interpolation) {
    renderer.drawSprite(this->sprite, this->position + (this->velocity * interpolation), this->size);
}
