#include "SpriteFrame.h"


SpriteFrame::SpriteFrame()
    : stride_x(1),stride_y(1){}
SpriteFrame::SpriteFrame(GLint im_width, GLint im_height, GLint frame_width, GLint frame_height, glm::vec2 index)
    : stride_x((GLfloat) frame_width/im_width), stride_y((GLfloat) frame_height/im_height), index(index), indexOrig(index)
    , hasMap(false), numFrames(0), frameIterator(0) {}

void SpriteFrame::setIndex(glm::vec2 index) {
    this->index = index;
}

glm::vec2 SpriteFrame::getIndexOrig() {
    return indexOrig;
}

glm::vec2 SpriteFrame::getIndex() {
    return index;
}

glm::vec4 SpriteFrame::getTextureCoords() {
    return glm::vec4(index.x*stride_x+0.002, index.y*stride_y+0.002, stride_x-0.004, stride_y-0.004);
}

void SpriteFrame::readMap(const GLchar* filename) {
    std::ifstream file(filename);
    std::string line;
	hasMap = true;
    if (file) {
        while(std::getline(file, line)) {
            std::istringstream iss(line);
            GLint x, y, sizex, sizey;
            while(iss >> x >> y >> sizex >> sizey) {
            	map.push_back(glm::vec2(x/sizex,y/sizey));
            }
            numFrames++;
        }
    }
}

void SpriteFrame::next(GLfloat step) {
	if (hasMap) {
		frameIterator += step;
		if (frameIterator>=numFrames) {
			frameIterator -= numFrames;
		}
		this->index = map[(GLint) frameIterator];
	} else {
		// TODO
	}
}
