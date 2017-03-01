#ifndef ICEBLOCK_H
#define ICEBLOCK_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Texture.h"

class Iceblock : public GameObject {
public:
	Iceblock(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite);

	void disintegrate() const;
	void slide();

private:
    // TODO: Add all ice-break textures and initialize the array in constructor
};

#endif // ICEBLOCK_H
