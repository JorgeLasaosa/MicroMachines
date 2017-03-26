#include "Wallblock.h"

Wallblock::Wallblock(glm::vec2 pos, glm::vec2 size, const Texture& sprite)
    : GameObject(pos, size, 0.0f, sprite, true), shaking(-1), odd(false),orientation(0) {}

void Wallblock::shake(GLint orientation, GLint odd)
{
    if (shaking==-1) {
    	shaking = 0;
    	this->odd = odd;
    	this->orientation = orientation;
	    changeIndexFrame(glm::vec2(1 + odd, orientation));
    }
}

void Wallblock::shine()
{
    // TODO: Implement Wallblock::shine()
}

void Wallblock::update() {
    if (shaking>=0) {
    	if (shaking%3 == 0){
	        changeIndexFrame(glm::vec2(1 + odd, orientation));
	        odd = (odd+1)%2;
    	}
    	shaking++;
    }
    if (shaking>=25) {
	    changeIndexFrame(this->frame.getIndexOrig());
    	shaking = -1;
    }
}