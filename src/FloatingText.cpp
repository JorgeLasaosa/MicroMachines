#include "FloatingText.h"

FloatingText::FloatingText(glm::vec2 pos, std::string text, GLint liveTime, GLfloat scale, glm::vec3 color)
	: pos(pos), text(text), liveTime(liveTime), scale(scale), color(color)
{
}

FloatingText::~FloatingText() {

}

GLboolean FloatingText::update() {
	liveTime--;
	return liveTime>0;
}

void FloatingText::draw() {
	ResourceManager::textRenderer->renderText(text, pos, scale, color);
}