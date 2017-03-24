#ifndef PLAYER_H
#define PLAYER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include "GameObject.h"
#include "Texture.h"

class Player : public GameObject {
public:
    glm::vec2 destination;

	Player(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, GLboolean isPushable = false);

	// Movements
    void move(Move move, GLfloat interpolation);
    void update();
};

#endif // PLAYER_H
