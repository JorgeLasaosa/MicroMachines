#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

#include <iostream>
// Game-related state data
SpriteRenderer* renderer;
GameObject* obj;

// Game Constructor
Game::Game(GLuint width, GLuint height) : WIDTH(width), HEIGHT(height){}

// Game Destructor
Game::~Game() {
    delete renderer;
    delete obj;
}

void Game::init() {
    // Load shaders
    ResourceManager::loadShaderFromFile("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");

    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    // Load textures
    ResourceManager::loadTexture("img/iceblock.png", GL_TRUE, "face");
    // Set Render-specific contols
    Shader spriteShader = ResourceManager::getShader("sprite");
    renderer = new SpriteRenderer(spriteShader);

    Texture faceTexture = ResourceManager::getTexture("face");
    obj = new GameObject(glm::vec3(200,200,0), glm::vec2(50,50), faceTexture);
}

void Game::update(GLfloat dt) {

}

void Game::proccessInput(GLfloat dt) {

}

void Game::render() {
    obj->draw(*renderer);
}
