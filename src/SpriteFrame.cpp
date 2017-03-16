#include "SpriteFrame.h"


SpriteFrame::SpriteFrame()
    : stride_x(1),stride_y(1){}
SpriteFrame::SpriteFrame(GLint im_width, GLint im_height, GLint frame_width, GLint frame_height, glm::vec2 index)
    : stride_x((GLfloat) frame_width/im_width), stride_y((GLfloat) frame_height/im_height), index(index) {}

void SpriteFrame::setIndex(glm::vec2 index) {
    this->index = index;
}

glm::vec4 SpriteFrame::getTextureCoords() {
    return glm::vec4(index.x*stride_x,index.y*stride_y,stride_x,stride_y);
}