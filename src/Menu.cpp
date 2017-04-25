#include "Menu.h"
#include "ResourceManager.h"

Menu::Menu(glm::vec2 pos)
    : pos(pos), selector(0), renderer(nullptr)
{}

Menu::Menu(glm::vec2 pos, SpriteRenderer* renderer)
    : pos(pos), selector(0), renderer(renderer)
{
    texture = ResourceManager::getTexture("pause-background");
    frame = SpriteFrame(texture.WIDTH, texture.HEIGHT, 128, 128, glm::vec2(0,0));
}

void Menu::drawMenu() {
    if (renderer != nullptr) {
        // Draw background
        renderer->drawSprite(texture, glm::vec3(pos.x,pos.y,0), size, frame);
    }
    GLfloat x = this->pos.x + 1.0f;
    GLfloat y = this->pos.y + 1.0f;
    for (MenuOption op : options) {
        ResourceManager::textRenderer->renderText(op.text, glm::vec2(x, y), 0.5f, op.color);
        y++;
    }

}

void Menu::setOptions(const std::vector<MenuOption>& menuOptions) {
    this->options = menuOptions;
    this->options[selector].color = glm::vec3(1.0f, 0.8f, 0.0f);
    GLint maxLength = 0;
    for (MenuOption op : menuOptions) {
        if (op.text.size() > maxLength) {
            maxLength = op.text.size();
        }
    }
    this->size.x = 1.0f + maxLength * 0.5f + 1.0f;
    this->size.y = 1.0f + menuOptions.size() * 1.0f + 1.0f;
}

void Menu::previousOption() {
    if (this->options[selector].active){
        this->options[selector].color = glm::vec3(0.0f, 1.0f, 1.0f);
    } else {
        this->options[selector].color = glm::vec3(0.5f, 0.5f, 0.5f);
    }
    this->selector = (this->selector + options.size() - 1) % options.size();
    if (this->options[selector].active){
        this->options[selector].color = glm::vec3(1.0f, 0.8f, 0.0f);
    } else {
        this->options[selector].color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

void Menu::nextOption() {
    if (this->options[selector].active){
        this->options[selector].color = glm::vec3(0.0f, 1.0f, 1.0f);
    } else {
        this->options[selector].color = glm::vec3(0.5f, 0.5f, 0.5f);
    }
    this->selector = (this->selector + options.size() + 1) % options.size();
    if (this->options[selector].active){
        this->options[selector].color = glm::vec3(1.0f, 0.8f, 0.0f);
    } else {
        this->options[selector].color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}

GLint Menu::getSelector() const {
    return this->selector;
}

Menu::~Menu()
{

}
