#ifndef MENU_H
#define MENU_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Menu {

    public:
        struct MenuOption {
            GLchar*     text;
            glm::vec3   color;
        };

        Menu(glm::vec2 pos, glm::vec2 size, glm::vec3 backgroundColor);
        void drawMenu() const;
        void setOptions(const std::vector<MenuOption>& menuOptions);
        void previousOption();
        void nextOption();
        GLint getSelector() const;
        virtual ~Menu();

    private:
        GLint selector;
        glm::vec2 pos, size;
        glm::vec3 backgroundColor;
        std::vector<MenuOption> options;

};

#endif // MENU_H
