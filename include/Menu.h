#ifndef MENU_H
#define MENU_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include "Shader.h"
#include "SpriteRenderer.h"
#include "Texture.h"

class Menu {

    public:
        struct MenuOption {
            std::string      text;
            glm::vec3   color;
        };
        std::vector<MenuOption> options;

        Menu(glm::vec2 pos);
        Menu(glm::vec2 pos, SpriteRenderer* renderer);
        void drawMenu();
        void setOptions(const std::vector<MenuOption>& menuOptions);
        void previousOption();
        void nextOption();
        GLint getSelector() const;
        virtual ~Menu();

    private:

        GLint selector;
        glm::vec2 pos, size;
        glm::vec3 backgroundColor;
        SpriteRenderer* renderer;
        Texture texture;
        SpriteFrame frame;

};

#endif // MENU_H
