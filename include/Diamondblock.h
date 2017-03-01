#ifndef DIAMONDBLOCK_H
#define DIAMONDBLOCK_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Texture.h"

class Diamondblock : public GameObject {
public:
	Diamondblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite);

	void shine() const;
	void slide();

private:
    // Normal and shiny sprites
    Texture sprites[2];

};

#endif // DIAMONDBLOCK_H
