#include "GameObject.h"

GameObject::GameObject()
    : position(0,0), size(1,1), sprite(), isPushable(false){}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, const Texture& sprite, GLboolean isPushable)
    : position(pos), size(size), sprite(sprite), isPushable(isPushable){}

GameObject::~GameObject() {
	//dtor
}

void GameObject::draw(SpriteRenderer& renderer) {
    renderer.drawSprite(this->sprite, this->position, this->size);
}
