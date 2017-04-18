#ifndef COMPONENT3D_H
#define COMPONENT3D_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "SpriteFrame.h"

class Component3D {
    public:
        Component3D* parent;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, const char* modelFile);
        // setPosition(glm::vec3 position);
        // setRototation(glm::vec3 rotation);
        // setScale(glm::vec3 scale);
        void drawModel(Texture& texture, glm::vec2 position, glm::vec3 rotation, GLfloat rotationAngle, glm::vec2 size, SpriteFrame frame);

        virtual ~Component3D();

    private:
        Shader shader;
        GLuint quadVAO;
        GLuint VBO_tex;
        GLint windowWidth, windowHeight;
        GLfloat squareSize;

        void initRenderData(const char* modelFile);
};

#endif // COMPONENT3D_H
