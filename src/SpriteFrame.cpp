#include "SpriteFrame.h"


SpriteFrame::SpriteFrame()
    : stride_x(1),stride_y(1){}
SpriteFrame::SpriteFrame(GLint im_width, GLint im_height, GLint frame_width, GLint frame_height, glm::vec2 index)
    : stride_x((GLfloat) frame_width/im_width), stride_y((GLfloat) frame_height/im_height), index(index), indexOrig(index) {}

void SpriteFrame::setIndex(glm::vec2 index) {
    this->index = index;
}

glm::vec2 SpriteFrame::getIndexOrig() {
	return indexOrig;
}

glm::vec4 SpriteFrame::getTextureCoords() {
    return glm::vec4(index.x*stride_x+0.001,index.y*stride_y+0.001,stride_x-0.002,stride_y-0.002);
}