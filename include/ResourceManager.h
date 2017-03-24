#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include "Texture.h"
#include "Shader.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined
class ResourceManager {
    private:
        // Private Constructor
        ResourceManager() {}

    public:
        // Resource storage
        static std::map<std::string, Shader> shaders;
        static std::map<std::string, Texture> textures;
        static GLint ticks;

        // Loads (and generates) a shader program from string loading vertex, fragment (and geometry)
        // shader's source code. If gShaderSource is not nullptr, it also loads a geometry shader
        static Shader loadShaderFromString(const GLchar* vShaderSource, const GLchar* fShaderSource,
                                           const GLchar* gShaderSource, std::string name);

        // Loads (and generates) a shader program from file loading vertex, fragment (and geometry)
        // shader's source code. If gShaderSource is not nullptr, it also loads a geometry shader
        static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile,
                                    const GLchar *gShaderFile, std::string name);

        // Retrieves a stored shader
        static Shader getShader(std::string name);

        // Loads (and generates) a texture from file
        static Texture loadTexture(const GLchar* file, GLboolean alpha, std::string name);

        // Retrieves a stored texture
        static Texture getTexture(std::string name);

        // Retrieves number of ticks
        static GLint getTicks();
        static void addTick();

        // Properly de-allocates all loaded resources
        static void clear();

};

#endif // RESOURCEMANAGER_H
