
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL/SOIL.h>

// Instantiate static variables
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
GLint ResourceManager::ticks = 0;
irrklang::ISoundEngine* ResourceManager::soundEngine = nullptr;
TextRenderer* ResourceManager::textRenderer = nullptr;

Shader ResourceManager::loadShaderFromString(const GLchar* vShaderSource, const GLchar* fShaderSource,
                                              const GLchar* gShaderSource, std::string name)
{
    Shader shader;
    shader.compile(vShaderSource, fShaderSource, gShaderSource != nullptr ? gShaderSource : nullptr);
    shaders[name] = shader;
    return shaders[name];
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile,
                                            const GLchar* gShaderFile, std::string name)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr) {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();

    // 2. Now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    shaders[name] = shader;
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
    return shaders[name];
}

Texture ResourceManager::loadTexture(const GLchar* file, GLboolean alpha, std::string name) {
    // Create Texture object
    Texture texture;
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // Load image
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    // Now generate texture
    texture.generate(width, height, image);

    // And finally free image data
    SOIL_free_image_data(image);
    textures[name] = texture;
    return textures[name];
}

Texture ResourceManager::getTexture(std::string name) {
    return textures[name];
}

void ResourceManager::initSound() {
    soundEngine = irrklang::createIrrKlangDevice();
}

void ResourceManager::initTextRenderer(Shader& shader, const GLuint windowWidth, const GLuint windowHeight) {
    textRenderer = new TextRenderer(shader, windowWidth, windowHeight);
}

void ResourceManager::clear() {
    // Properly delete all shaders
    for (auto &iter : shaders) {
        glDeleteProgram(iter.second.program);
    }
    // Properly delete all textures
    for (auto &iter : textures) {
        glDeleteTextures(1, &iter.second.ID);
    }
    soundEngine->drop();
    delete textRenderer;
}

GLint ResourceManager::getTicks() {
    return ticks;
}

void ResourceManager::addTick() {
    ticks = (ticks+1) % 25;
}

