#include "Menu.h"
#include "ResourceManager.h"

Menu::Menu(glm::vec2 pos, glm::vec3 backgroundColor)
    : pos(pos), backgroundColor(backgroundColor), selector(0)
{}

void Menu::drawMenu() const {
    GLfloat x = this->pos.x + 0.5f;
    GLfloat y = this->pos.y + 0.5f;
    for (MenuOption op : options) {
        ResourceManager::textRenderer->renderText(op.text, glm::vec2(x, y), 0.5f, op.color);
        y++;
    }
}

void Menu::setOptions(const std::vector<MenuOption>& menuOptions) {
    this->options = menuOptions;
    this->options[selector].color = glm::vec3(1.0f, 0.8f, 0.0f);
}

void Menu::previousOption() {
    this->options[selector].color = glm::vec3(0.0f, 1.0f, 1.0f);
    this->selector = (this->selector + options.size() - 1) % options.size();
    this->options[selector].color = glm::vec3(1.0f, 0.8f, 0.0f);
}

void Menu::nextOption() {
    this->options[selector].color = glm::vec3(0.0f, 1.0f, 1.0f);
    this->selector = (this->selector + options.size() + 1) % options.size();
    this->options[selector].color = glm::vec3(1.0f, 0.8f, 0.0f);
}

GLint Menu::getSelector() const {
    return this->selector;
}

Menu::~Menu()
{

}
