#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class SpriteRenderer {

    private:
        // Render state
        Shader shader;
        GLuint quadVAO;

        // Initializes and configures the quad's buffer and vertez attributes
        void initRenderData();
    public:
        // Constructor (inits shaders/shapes
        SpriteRenderer(Shader& shader);

        // Destructor
        virtual ~SpriteRenderer();
};

#endif // SPRITERENDERER_H
