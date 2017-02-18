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
        glm::vec3 position;
        glm::vec2 size, speed;
        GLboolean destroyed;

        // Render state
        Texture sprite;

        // Constructor(s)
        GameObject();
        GameObject(glm::vec3 pos, glm::vec2 size, const Texture& sprite,
                    glm::vec2 speed = glm::vec2(0.0f, 0.0f));
        virtual ~GameObject();

        // Draw sprite
        virtual void draw(SpriteRenderer& renderer);

    private:
};

#endif // GAMEOBJECT_H
