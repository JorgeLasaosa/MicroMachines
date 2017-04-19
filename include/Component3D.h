#ifndef COMPONENT3D_H
#define COMPONENT3D_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh3DRenderer.h"

class Component3D {
    public:
        Component3D* parent;
        Mesh3DRenderer* mesh;

        glm::vec3 position;
        glm::vec3 rotation;
        GLfloat rotationAngle;
        glm::vec3 scale;

        Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, const char* modelFile);
        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation, GLfloat rotationAngle);
        void setScale(glm::vec3 scale);
        void draw();

        virtual ~Component3D();
};

#endif // COMPONENT3D_H
