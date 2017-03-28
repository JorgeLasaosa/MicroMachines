#ifndef SPRITEFRAME_H
#define SPRITEFRAME_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

class SpriteFrame {
		GLfloat stride_x, stride_y;
		glm::vec2 index;
		glm::vec2 indexOrig;
		std::vector <glm::vec2> map;
		bool hasMap;
		GLint numFrames;
		GLfloat frameIterator;
    public:
        SpriteFrame();
        SpriteFrame(GLint im_width, GLint im_height, GLint frame_width, GLint frame_height, glm::vec2 index=glm::vec2(0,0));
        void setIndex(glm::vec2 index);
        void readMap(const GLchar*filename);
        void next(GLfloat step);
        glm::vec2 getIndexOrig();
        glm::vec2 getIndex();
        glm::vec4 getTextureCoords();
};

#endif // SPRITEFRAME_H
