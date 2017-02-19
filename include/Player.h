#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject {
public:
	Player(glm::vec2 pos, glm::vec2 size, const Texture& sprite, GLboolean isPushable = false);

	// Movements
    void move(glm::vec2 move, const Texture& texture);

private:
};

#endif // PLAYER_H
