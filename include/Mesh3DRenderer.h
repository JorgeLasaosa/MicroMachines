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
    private:
        Shader shader;
        GLuint quadVAO;
        GLuint VBO_tex;
        Camera* camera;

        void initRenderData(const char* modelFile);
        void initRenderDataPhong(const char* modelFile);
    public:
        GLint numVertices;
        GLint numFaces;

        Mesh3DRenderer(Shader& shader, const char* modelFile, Camera* camera);
        void draw(glm::mat4 model,glm::vec3 objPos);
        void setShader(Shader& shader);

        virtual ~Mesh3DRenderer();
};

#endif // MESH3DRENDERER_H
