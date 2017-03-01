#include "Diamondblock.h"
#include "ResourceManager.h"

Diamondblock::Diamondblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite)
	: GameObject(pos, size, velocity, sprite, true)
{
    sprites[0] = ResourceManager::getTexture("diamond");
    sprites[1] = ResourceManager::getTexture("diamond-shiny");
}

void Diamondblock::shine() const {
    // TODO Implement Diamondblock::shine()
}

void Diamondblock::slide() {
    // TODO Implement Diamondblock::slide()
}
