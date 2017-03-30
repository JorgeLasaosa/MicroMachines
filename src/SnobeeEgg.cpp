#include "SnobeeEgg.h"

SnobeeEgg::SnobeeEgg(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type)
    : GameObject(pos, size, velocity, initialSprite, false, SHAPE_RECTANGLE), type(type), state(SHOWING), step(0)
{
}

SnobeeEgg::~SnobeeEgg() {
}

bool SnobeeEgg::update(GameLevel* level) {
	if (state == SHOWING && step >= 13) {
		step = 0;
		state = BREAKING1;
		frame.setIndex(frame.getIndexOrig() + glm::vec2(1,0));
	}
	if (state == BREAKING1 && step >= 15) {
		step = 0;
		state = BREAKING2;
		frame.setIndex(frame.getIndexOrig() + glm::vec2(2,0));
	}
	if (state == BREAKING2 && step >= 15) {
		step = 0;
		state = BORN;
		frame.setIndex(frame.getIndexOrig() + glm::vec2(3,0));
        // Enemies

        Snobee* enem = new Snobee(position, size, velocity, level->creaturesTexture, type);//glm::vec2(0.5f, 2.0f)
        enem->configureFrame(160, 160, glm::vec2(0,9));
        level->enemies.push_back(enem);
	}
	if (state == BORN && step >= 20) {
		return false;
	}

	step++;
	return true;
}
