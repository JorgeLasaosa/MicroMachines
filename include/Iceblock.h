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
	GLfloat interp_frame;
	bool isEggBlock, destroyByPengo;

	Iceblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite);
	virtual ~Iceblock();
	void keepDisintegrate(GLfloat interpolation);
	void disintegrate(GameLevel* level, bool destroyByPengo);
	void slide(Move move, GameLevel* level);
private:
};

#endif // ICEBLOCK_H
