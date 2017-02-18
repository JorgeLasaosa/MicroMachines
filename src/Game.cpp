#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"

#include <iostream>
#include <vector>

// Game-related state data
SpriteRenderer* renderer;
GameObject* obj;

GameLevel* level0;

// Game Constructor
Game::Game(GLuint width, GLuint height) : WIDTH(width), HEIGHT(height){}

// Game Destructor
Game::~Game() {
    delete renderer;
    delete obj;
    delete level0;
}

void Game::init() {
    // Load shaders
    ResourceManager::loadShaderFromFile("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");

    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    // Load textures
    ResourceManager::loadTexture("img/pengo/pengo0.png", GL_TRUE, "pengo0");
    ResourceManager::loadTexture("img/walls/wall0.png", GL_TRUE, "wall0");
    ResourceManager::loadTexture("img/walls/wall1.png", GL_TRUE, "wall1");
    ResourceManager::loadTexture("img/iceblock/iceblock.png", GL_TRUE, "iceblock");
    ResourceManager::loadTexture("img/diamond/diamond.png", GL_TRUE, "diamond");

    // Set Render-specific contols
    Shader spriteShader = ResourceManager::getShader("sprite");
    renderer = new SpriteRenderer(spriteShader);

    // Pengo
    Texture pengoTexture = ResourceManager::getTexture("pengo0");
    obj = new GameObject(glm::vec2(350,400), glm::vec2(40,40), pengoTexture);

    level0 = new GameLevel();
    level0->load("levels/level0.txt");
}

void Game::update(GLfloat dt) {

}

void Game::proccessInput(GLfloat dt) {

}

void Game::render() {
    obj->draw(*renderer);
    level0->draw(*renderer);
}
