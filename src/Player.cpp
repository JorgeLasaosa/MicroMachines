

#include "Player.h"
#include "ResourceManager.h"

Player::Player(glm::vec2 pos, glm::vec2 size, const Texture& sprite, GLboolean isPushable)
	: GameObject(pos, size, sprite, isPushable) {}

void Player::move(glm::vec2 move, const Texture& texture) {
    this->position += move;
    this->sprite = texture;
}

