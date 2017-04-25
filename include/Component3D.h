#ifndef COMPONENT3D_H
#define COMPONENT3D_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

#include "Mesh3DRenderer.h"

class Component3D {
    public:
        Component3D* parent;
        std::vector<Component3D*> childs;
        Mesh3DRenderer* mesh;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        GLfloat shear;
        GLboolean zup;

        Component3D(Mesh3DRenderer* mesh, GLboolean zup=false);

        void setPosition(glm::vec3 position);
        void setRotation(glm::vec3 rotation);
        void setScale(glm::vec3 scale);
        void setShear(GLfloat shear);
        glm::mat4 getTransormMatrix(glm::mat4 model);

        void setParent(Component3D* parent);

        void draw(GLboolean drawChilds = true);

        virtual ~Component3D();
    private:
        GLfloat squareSize;
};

#endif // COMPONENT3D_H
