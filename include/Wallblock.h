#ifndef WALLBLOCK_H
#define WALLBLOCK_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Texture.h"

class Wallblock : public GameObject {
public:
	Wallblock(glm::vec2 pos, glm::vec2 size, const Texture& sprite);

	// Shakes when Pengo pushes it
	void shake();
	// Shines when all three diamond blocks are aligned
	void shine();

private:
    // TODO: Array with wall shaking textures
};

#endif // WALLBLOCK_H
