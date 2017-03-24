#include "Snobee.h"

Snobee::Snobee(glm::vec2 pos, glm::vec2 size, GLfloat velocity, const Texture& initialSprite, SnobeeType type)
    : GameObject(pos, size, velocity, initialSprite, false, SHAPE_RECTANGLE), type(type), isMoving(false)
{
}

//Snobee::move() {
//    if (!isMoving) {
//        switch(this->type) {
//            case GREEN:
//                break;
//            case YELLOW:
//                break;
//            case RED:
//                break;
//        }
//    }
//}

Snobee::~Snobee() {

}
