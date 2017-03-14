#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Game.h"

enum State {
    MOVING,
    STOPPED,
    DEADING,
    DEAD,
};

enum Move {
    MOVE_UP,
    MOVE_LEFT,
    MOVE_DOWN,
    MOVE_RIGHT,
};

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
    public:

        // Object state
        glm::vec2 position, origPos, size, destination;
        GLboolean isPushable;
        GLfloat velocity;
        Move movement;
        State state;

        // Render state
        Texture sprite;

        // Constructor(s)
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& sprite, GLboolean isPushable = false);
        virtual ~GameObject();

        // Actions
        bool move(GLfloat interpolation);

        // Draw sprite
        virtual void draw(SpriteRenderer& renderer);
        virtual void draw(SpriteRenderer& renderer, GLfloat interpolation);
};

#endif // GAMEOBJECT_H
