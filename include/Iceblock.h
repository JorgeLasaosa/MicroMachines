#ifndef ICEBLOCK_H
#define ICEBLOCK_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameLevel.h"
#include "GameObject.h"
#include "Texture.h"
#include <iostream>

class GameLevel;// Empty class for compile
class Iceblock : public GameObject {
public:
	Iceblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite);
	GLfloat interp_frame;
	bool isEggBlock, destroyByPengo;

	void keepDisintegrate(GLfloat interpolation);
	void disintegrate(GameLevel* level, bool destroyByPengo);
	void slide(Move move, GameLevel* level);
private:
    // TODO: Add all ice-break textures and initialize the array in constructor
};

#endif // ICEBLOCK_H
