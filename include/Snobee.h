#ifndef SNOBEE_H
#define SNOBEE_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "GameLevel.h"

#ifndef SNOBEETYPE
#define SNOBEETYPE
enum SnobeeType {
    GREEN,
    YELLOW,
    RED
};
#endif

class Snobee : public GameObject {
    public:
        SnobeeType type;
        glm::vec2 destination;
        GLboolean isMoving;
        GLboolean isNumb;

        Snobee(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type);
        //void move();
        void numb(GLboolean isNumb=true);
        GLboolean nextMove(GameLevel* level, GLboolean comeBack=false);


        virtual ~Snobee();

    private:
};

#endif // SNOBEE_H
