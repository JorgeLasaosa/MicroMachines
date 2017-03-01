#include "Iceblock.h"

Iceblock::Iceblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite)
	: GameObject(pos, size, velocity, sprite, true) {}

void Iceblock::disintegrate() const {
    // TODO Implement Iceblock::disintegrate()
}

void Iceblock::slide() {
    // TODO Implement Iceblock::slide()
}
