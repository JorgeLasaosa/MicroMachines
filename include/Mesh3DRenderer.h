#ifndef MESH3DRENDERER_H
#define MESH3DRENDERER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "SpriteFrame.h"
#include "Camera.h"

class Mesh3DRenderer {
    public:
        GLint numVertices;
        GLint numFaces;

        Mesh3DRenderer(Shader& shader, const char* modelFile, const GLint windowWidth, const GLint windowHeight);
        void draw(glm::mat4 model, Camera* camera);

        virtual ~Mesh3DRenderer();

        Shader shader;
        GLuint quadVAO;
        GLuint VBO_tex;
        GLint windowWidth, windowHeight;

        void initRenderData(const char* modelFile);
};

#endif // MESH3DRENDERER_H
