#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ResourceManager.h"

class FloatingText {
    public:

        FloatingText(glm::vec2 pos, std::string text, GLint liveTime, GLfloat scale, glm::vec3 color, Camera* camera);
        void draw();
        GLboolean update();

        virtual ~FloatingText();

    private:
        GLint liveTime;
        std::string text;
        glm::vec2 pos;
        GLfloat scale;
        glm::vec3 color;
        Camera* camera;
};

#endif // FLOATINGTEXT_H
