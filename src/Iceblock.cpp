#include "Iceblock.h"

Iceblock::Iceblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite)
	: GameObject(pos, size, velocity, sprite, true) {}

void Iceblock::disintegrate() const {
    // TODO Implement Iceblock::disintegrate()
}

void Iceblock::slide(Move move, GameLevel* level) {
	if(!active) {
	    int xOffset = 100, yOffset = 40;
	    origPos = position;
		active = true;
		movement = move;
		level->activeObjects.push_back(this);
        int b = (position.x - xOffset+20)/40 - 1;
        int a = (position.y - yOffset+20)/40 - 1;
		switch(move) {
			case MOVE_UP: {
				bool found = false;
				for(float i = position.y-40; !found; i-=40) {
					if(level->checkCollision(glm::vec2(position.x,i))) {
						found = true;
						destination = glm::vec2(position.x,i+40);
					}
				}
			}
			break;
			case MOVE_DOWN: {
				bool found = false;
				for(float i = position.y+40; !found; i+=40) {
					if(level->checkCollision(glm::vec2(position.x,i))) {
						found = true;
						destination = glm::vec2(position.x,i-40);
					}
				}
			}
			break;
			case MOVE_LEFT: {
				bool found = false;
				for(float i = position.x-40; !found; i-=40) {
					if(level->checkCollision(glm::vec2(i,position.y))) {
						found = true;
						destination = glm::vec2(i+40, position.y);
					}
				}
			}
			break;
			case MOVE_RIGHT: {
				bool found = false;
				for(float i = position.x+40; !found; i+=40) {
					if(level->checkCollision(glm::vec2(i,position.y))) {
						found = true;
						destination = glm::vec2(i-40, position.y);
					}
				}
			}
			break;
		}
	}
}
