#include "FloatingText.h"
#include "Game.h"

FloatingText::FloatingText(glm::vec2 pos, std::string text, GLint liveTime, GLfloat scale, glm::vec3 color, Camera* camera)
	: pos(pos), text(text), liveTime(liveTime), scale(scale), color(color), camera(camera)
{
}

FloatingText::~FloatingText() {

}

GLboolean FloatingText::update() {
	liveTime--;
	return liveTime>0;
}

void FloatingText::draw() {
	if (Game::mode3D){
		// View Matrix
		ResourceManager::getShader("text").use().setMatrix4("view", this->camera->getViewMatrix());
		glm::mat4 projection = this->camera->getPerspective();
    	ResourceManager::getShader("text").use().setMatrix4("projection", projection);
		ResourceManager::textRenderer->renderText(text, glm::vec3(pos.x-0.5f, 3.5f - liveTime/20.0f, pos.y-0.5f), scale, color);
		//ResourceManager::textRenderer->renderText(text, pos, scale, color);
	}
	else {
		ResourceManager::textRenderer->renderText(text, pos, scale, color);
	}
	glm::mat4 projection = this->camera->getOrthogonal();
	ResourceManager::getShader("text").use().setMatrix4("projection", projection);
	ResourceManager::getShader("text").use().setMatrix4("view", glm::mat4());
}