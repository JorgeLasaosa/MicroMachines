#ifndef SNOBEEEGG_H
#define SNOBEEEGG_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Snobee.h"
#include "GameLevel.h"

enum EggState {
    SHOWING,
    BREAKING1,
    BREAKING2,
    BORN,
};

class SnobeeEgg : public GameObject {
    public:
        SnobeeType type;
        EggState state;
        GLint step;
        
        SnobeeEgg(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type);
        virtual ~SnobeeEgg();

        bool update(GameLevel* level);

    private:
};

#endif // SNOBEEEGG_H
