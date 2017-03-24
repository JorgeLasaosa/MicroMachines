#ifndef DIAMONDBLOCK_H
#define DIAMONDBLOCK_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "GameLevel.h"
#include "Texture.h"

class GameLevel;// Empty class for compile
class Diamondblock : public GameObject {
public:
	Diamondblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite);

	void shine() const;
	void slide(Move move, GameLevel* level);

private:
    // Normal and shiny sprites
    Texture sprites[2];

};

#endif // DIAMONDBLOCK_H
