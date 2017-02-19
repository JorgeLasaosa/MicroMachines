#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
    public:
        // Object state
        glm::vec2 position, size;
        GLboolean isPushable;

        // Render state
        Texture sprite;

        // Constructor(s)
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, const Texture& sprite, GLboolean isPushable = false);
        virtual ~GameObject();

        // Draw sprite
        virtual void draw(SpriteRenderer& renderer);

    private:
};

#endif // GAMEOBJECT_H
