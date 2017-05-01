#ifndef CUBERENDERER_H
#define CUBERENDERER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "SpriteFrame.h"
#include "Camera.h"

class Cube3DRenderer {
    public:
        Cube3DRenderer(Shader& shader, const GLint windowWidth, const GLint windowHeight, Camera* camera);
        virtual ~Cube3DRenderer();

        void drawCube(Texture& texture, glm::vec3 position, glm::vec3 size, SpriteFrame frame);
        void setShader(Shader shader);
    private:
        Shader shader;
        GLuint cubeVAO;
        GLint windowWidth, windowHeight;
        GLfloat squareSize;
        Camera* camera;

        void initRenderData();
        void initRenderDataPhong();
};

#endif // CUBERENDERER_H
