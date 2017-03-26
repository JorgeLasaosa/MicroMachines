#ifndef WALLBLOCK_H
#define WALLBLOCK_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Texture.h"

class Wallblock : public GameObject {
public:
	Wallblock(glm::vec2 pos, glm::vec2 size, const Texture& sprite);
	GLint shaking;
	GLint orientation;
	GLint odd;

	// Shakes when Pengo pushes it
	void shake(GLint orientation, GLint odd);
	// Shines when all three diamond blocks are aligned
	void shine();

	void update();
private:
    // TODO: Array with wall shaking textures
};

#endif // WALLBLOCK_H
