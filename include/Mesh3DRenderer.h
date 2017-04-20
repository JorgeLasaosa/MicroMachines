#ifndef MESH3DRENDERER_H
#define MESH3DRENDERER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Texture.h"
#include "Shader.h"
#include "SpriteFrame.h"

class Mesh3DRenderer {
    public:
        GLint numVertices;
        GLint numFaces;

        Mesh3DRenderer(Shader& shader, const char* modelFile);
        void draw(glm::mat4 model);

        virtual ~Mesh3DRenderer();

        Shader shader;
        GLuint quadVAO;
        GLuint VBO_tex;

        void initRenderData(const char* modelFile);
};

#endif // MESH3DRENDERER_H
