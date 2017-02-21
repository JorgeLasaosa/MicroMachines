#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"
#include "Player.h"

#include <iostream>
#include <vector>
#include <time.h>

// Game-related state data
SpriteRenderer* renderer;

GameLevel* level1;
Player* player;

Texture moveDownTextures[2];
Texture moveLeftTextures[2];
Texture moveUpTextures[2];
Texture moveRightTextures[2];

GLuint indexTex = 0;

GLfloat accumulated = 0.0;

glm::vec2 lastMove;

// Game Constructor
Game::Game(GLuint width, GLuint height) : WIDTH(width), HEIGHT(height) {}

// Game Destructor
Game::~Game() {
	delete renderer;
	delete level1;
	delete player;
}

void Game::init() {
	// Load shaders
	ResourceManager::loadShaderFromFile("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);

	// Load textures
	moveDownTextures[0] = ResourceManager::loadTexture("img/pengo/pengo0.png", GL_TRUE, "pengoDown0");
	moveDownTextures[1] = ResourceManager::loadTexture("img/pengo/pengo1.png", GL_TRUE, "pengoDown1");
	moveLeftTextures[0] =ResourceManager::loadTexture("img/pengo/pengo2.png", GL_TRUE, "pengoLeft0");
	moveLeftTextures[1] =ResourceManager::loadTexture("img/pengo/pengo3.png", GL_TRUE, "pengoLeft1");
	moveUpTextures[0] = ResourceManager::loadTexture("img/pengo/pengo4.png", GL_TRUE, "pengoUp0");
	moveUpTextures[1] = ResourceManager::loadTexture("img/pengo/pengo5.png", GL_TRUE, "pengoUp1");
	moveRightTextures[0] = ResourceManager::loadTexture("img/pengo/pengo6.png", GL_TRUE, "pengoRight0");
	moveRightTextures[1] = ResourceManager::loadTexture("img/pengo/pengo7.png", GL_TRUE, "pengoRight1");
	ResourceManager::loadTexture("img/walls/wall0.png", GL_TRUE, "wall0");
	ResourceManager::loadTexture("img/walls/wall1.png", GL_TRUE, "wall1");
	ResourceManager::loadTexture("img/iceblock/iceblock.png", GL_TRUE, "iceblock");
	ResourceManager::loadTexture("img/diamond/diamond.png", GL_TRUE, "diamond");

	// Set Render-specific contols
	Shader spriteShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(spriteShader);

	level1 = new GameLevel();
	level1->load("levels/level1.txt");

	player = level1->pengo;
}

void Game::update() {

}

void Game::proccessInput() {
	if (this->state == GAME_ACTIVE) {
		GLfloat velocity = 40.0f / 25.0f;

		// Move playerboard
		if (this->keys[GLFW_KEY_UP]) {
            lastMove = glm::vec2(0, -velocity);
            player->move(lastMove, moveUpTextures[indexTex]);
		}

		else if (this->keys[GLFW_KEY_DOWN]) {
            lastMove = glm::vec2(0, velocity);
            player->move(lastMove, moveDownTextures[indexTex]);
		}

		else if (this->keys[GLFW_KEY_LEFT]) {
            lastMove = glm::vec2(-velocity, 0);
            player->move(lastMove, moveLeftTextures[indexTex]);
		}

		else if (this->keys[GLFW_KEY_RIGHT]) {
            lastMove = glm::vec2(velocity, 0);
            player->move(lastMove, moveRightTextures[indexTex]);
		}
	}
}

void Game::render(GLfloat interpolation) {
	level1->draw(*renderer);
	player->move(lastMove * interpolation, moveRightTextures[indexTex]);
	player->draw(*renderer);
}
