#include "GameObject.h"

GameObject::GameObject()
    : position(0,0), size(1,1), speed(0.0f), sprite(), destroyed(false){}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, const Texture& sprite, glm::vec2 speed)
    : position(pos), size(size), speed(speed), sprite(sprite), destroyed(false){}

GameObject::~GameObject() {
	//dtor
}

void GameObject::draw(SpriteRenderer& renderer) {
    renderer.drawSprite(this->sprite, this->position, this->size);
}
