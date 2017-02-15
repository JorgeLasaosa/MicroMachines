#ifndef MAP_H
#define MAP_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

class Map {
    public:
        Map(Shader& shader);
        virtual ~Map();

        void drawMap(Texture& texture, glm::vec3 position, glm::vec2 size);
    private:
        Shader shader;
        GLuint quadVAO;
        void initRenderData();
};

#endif // MAP_H
