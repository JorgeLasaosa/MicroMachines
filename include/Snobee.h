#ifndef SNOBEE_H
#define SNOBEE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "GameObject.h"

enum SnobeeType {
    GREEN,
    YELLOW,
    RED
};

class Snobee : public GameObject {
    public:
        SnobeeType type;
        glm::vec2 destination;
        GLboolean isMoving;

        Snobee(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type);
        //void move();
        virtual ~Snobee();

    private:
};

#endif // SNOBEE_H
