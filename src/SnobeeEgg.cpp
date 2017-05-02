#include "SnobeeEgg.h"

SnobeeEgg::SnobeeEgg(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type)
    : GameObject(pos, size, velocity, initialSprite, false, SHAPE_DOT), type(type), state(SHOWING), step(0)
{
    Component3D* huevo1 = new Component3D(ResourceManager::getMesh("snobeeEgg"), false);
    Component3D* huevo2 = new Component3D(ResourceManager::getMesh("snobeeEgg"), false);
    huevo2->setParent(huevo1);// Child 0
    GLfloat scale = 12*(Game::windowHeight / 18.0f)/MAP_SCALE;
    huevo1->setPosition(glm::vec3(pos.x, 0 ,pos.y) * (Game::windowHeight / 18.0f));
    huevo1->setScale(glm::vec3(1,1,1) * scale);
    setComp3D(huevo1);
}

SnobeeEgg::~SnobeeEgg() {
}

bool SnobeeEgg::update(GameLevel* level) {
	frame3D += 3;
	if (state == SHOWING && step < 13) {
		frame3D = 0;
	} else if (state == BREAKING1 && step < 13) {
		frame3D = 3;
	}
	if (state == SHOWING && step >= 13) {
		step = 0;
		state = BREAKING1;
		frame.setIndex(frame.getIndexOrig() + glm::vec2(1,0));
	}
	if (state == BREAKING1 && step >= 15) {
		step = 0;
		state = BREAKING2;
		frame.setIndex(frame.getIndexOrig() + glm::vec2(2,0));
		
        // Enemies
        snobee = new Snobee(position, size, velocity, level->creaturesTexture, type);//glm::vec2(0.5f, 2.0f)
        snobee->configureFrame(160, 160, glm::vec2(0,9));
		snobee->state = BORNING;
        level->enemies.push_back(snobee);
	}
	if (state == BREAKING2 && step >= 15) {
		step = 0;
		state = BORN;
		frame.setIndex(frame.getIndexOrig() + glm::vec2(3,0));
		snobee->state = STOPPED;
	}
	if (state == BORN && step >= 15) {
		frame3D++;
		return false;
	}
	if (frame3D*2>90) frame3D = 45;

    component3D->setRotation(glm::vec3(0,0,-frame3D*2));
    component3D->childs[0]->setRotation(glm::vec3(0,180,-frame3D*4));

	step++;
	return true;
}
