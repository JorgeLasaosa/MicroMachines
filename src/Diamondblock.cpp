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

void Diamondblock::slide(Move move, GameLevel* level) {
	if(state!=MOVING) {
	    origPos = position;
		state = MOVING;
		movement = move;
		level->activeObjects.push_back(this);
		switch(move) {
			case MOVE_UP: {
				bool found = false;
				for(float i = position.y-1; !found; i--) {
					if(level->checkCollision(glm::vec2(position.x,i))) {
						found = true;
						destination = glm::vec2(position.x,i+1);
					}
				}
			}
			break;
			case MOVE_DOWN: {
				bool found = false;
				for(float i = position.y+1; !found; i++) {
					if(level->checkCollision(glm::vec2(position.x,i))) {
						found = true;
						destination = glm::vec2(position.x,i-1);
					}
				}
			}
			break;
			case MOVE_LEFT: {
				bool found = false;
				for(float i = position.x-1; !found; i--) {
					if(level->checkCollision(glm::vec2(i,position.y))) {
						found = true;
						destination = glm::vec2(i+1, position.y);
					}
				}
			}
			break;
			case MOVE_RIGHT: {
				bool found = false;
				for(float i = position.x+1; !found; i++) {
					if(level->checkCollision(glm::vec2(i,position.y))) {
						found = true;
						destination = glm::vec2(i-1, position.y);
					}
				}
			}
			break;
		}
	}
}
