#ifndef SPRITEFRAME_H
#define SPRITEFRAME_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpriteFrame {
		GLfloat stride_x, stride_y;
		glm::vec2 index;
    public:
        SpriteFrame();
        SpriteFrame(GLint im_width, GLint im_height, GLint frame_width, GLint frame_height, glm::vec2 index=glm::vec2(0,0));
        void setIndex(glm::vec2 index);
        glm::vec4 getTextureCoords();
};

#endif // SPRITEFRAME_H
