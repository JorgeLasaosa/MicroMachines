#ifndef COMPONENT3D_H
#define COMPONENT3D_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh3DRenderer.h"

class Component3D {
    public:
        Component3D* parent;
        std::vector<Component3D*> childs;
        Mesh3DRenderer* mesh;

        glm::vec3 position;
        glm::vec3 rotation;
        GLfloat rotationAngle;
        glm::vec3 scale;

        Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, const char* modelFile);
        Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, Mesh3DRenderer* mesh);

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation, GLfloat rotationAngle);
        void setScale(glm::vec3 scale);
        glm::mat4 getTransormMatrix(glm::mat4 model);

        void setParent(Component3D* parent);

        void draw();

        virtual ~Component3D();
    private:
        GLfloat squareSize;
};

#endif // COMPONENT3D_H
