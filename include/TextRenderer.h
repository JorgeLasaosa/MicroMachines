#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

// GLFW and GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include "Shader.h"
#include "Texture.h"

struct Character {
    Texture    texture;    // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

class TextRenderer {

    public:
        TextRenderer(Shader& shader, const GLuint windowWidth, const GLuint windowHeight);
        void renderText(std::string text, glm::vec2 position, GLfloat scale, glm::vec3 color);
        virtual ~TextRenderer();

    private:
        Shader shader;
        GLuint quadVAO, quadVBO;
        GLuint windowWidth, windowHeight;
        GLfloat squareSize;
        std::map<GLchar, Character> characters;
        void initRenderData();
};

#endif // TEXTRENDERER_H
