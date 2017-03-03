#ifndef PLAYER_H
#define PLAYER_H

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
    void move(const std::string& move);

private:
    Texture moveUpTextures[2];
    Texture moveDownTextures[2];
    Texture moveRightTextures[2];
    Texture moveLeftTextures[2];
};

#endif // PLAYER_H
