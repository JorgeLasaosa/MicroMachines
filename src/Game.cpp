
#include "Game.h"
#include "ResourceManager.h"
#include "MusicHandler.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"
#include "Player.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <string>

// Game-related state data
SpriteRenderer* renderer;

GameLevel* level;
Player* player;

// Player move (up, down, left, right)
std::string playerMove;

// Game Constructor

Game::Game(GLuint width, GLuint height) : WIDTH(width), HEIGHT(height) {
	time_step = 0;
}

// Game Destructor
Game::~Game() {
	delete renderer;
	delete level;
}

void Game::init() {
	// Load shaders
	ResourceManager::loadShaderFromFile("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);

	// Load textures
	ResourceManager::loadTexture("img/pengo/pengo0.png", GL_TRUE, "pengoDown0");
	ResourceManager::loadTexture("img/pengo/pengo1.png", GL_TRUE, "pengoDown1");
	ResourceManager::loadTexture("img/pengo/pengo2.png", GL_TRUE, "pengoLeft0");
	ResourceManager::loadTexture("img/pengo/pengo3.png", GL_TRUE, "pengoLeft1");
	ResourceManager::loadTexture("img/pengo/pengo4.png", GL_TRUE, "pengoUp0");
	ResourceManager::loadTexture("img/pengo/pengo5.png", GL_TRUE, "pengoUp1");
	ResourceManager::loadTexture("img/pengo/pengo6.png", GL_TRUE, "pengoRight0");
	ResourceManager::loadTexture("img/pengo/pengo7.png", GL_TRUE, "pengoRight1");
	ResourceManager::loadTexture("img/walls/wall0.png", GL_TRUE, "wall0");
	ResourceManager::loadTexture("img/walls/wall1.png", GL_TRUE, "wall1");
	ResourceManager::loadTexture("img/iceblock/iceblock.png", GL_TRUE, "iceblock");
	ResourceManager::loadTexture("img/diamond/diamond.png", GL_TRUE, "diamond");
	ResourceManager::loadTexture("img/diamond/diamond-shiny.png", GL_TRUE, "diamond-shiny");

	// Set Render-specific contols
	Shader spriteShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(spriteShader);

	level = new GameLevel();
	level->load("levels/level1.txt");

	player = level->pengo;

	// Play music
	MusicHandler::init();
	MusicHandler::loadSound("sounds/level.wav","level");
	MusicHandler::loadSound("sounds/create_level.wav","create_level");
	MusicHandler::loadSound("sounds/death.wav","death");
	MusicHandler::loadSound("sounds/init_level.wav","init_level");
	MusicHandler::loadSound("sounds/insert_coin.wav","insert_coin");// TODO Neceasrio?

	MusicHandler::play("create_level");

}

void Game::update() {
    if (player->position != player->destination) {
        player->move(playerMove);
	}
    ResourceManager::addTick();
	if(this->state == GAME_GEN_LEVEL) {
		time_step += 1;
		bool end = true;
		if (time_step>0) {
			time_step = 0;
		    for (auto &i : level->fieldStart) {
		        for (auto &j : i) {
		            if (j != nullptr && end) {
		                j = nullptr;
		                end = false;
		            }
		        }
		    }
			if (end) {
				MusicHandler::play("level");
				this->state = GAME_ACTIVE;
			}
		}
	}
}

void Game::proccessInput() {
	if (this->state == GAME_ACTIVE && player->position == player->destination) {

		// Move playerboard
		if (this->keys[GLFW_KEY_UP] >= GLFW_PRESS) {
            playerMove = "up";
            player->destination = player->position + glm::vec2(0, -player->size.y);
		}

		if (this->keys[GLFW_KEY_DOWN] >= GLFW_PRESS) {
            playerMove = "down";
            player->destination = player->position + glm::vec2(0, player->size.y);
		}

		if (this->keys[GLFW_KEY_LEFT] >= GLFW_PRESS) {
            playerMove = "left";
            player->destination = player->position + glm::vec2(-player->size.x, 0);
		}

		if (this->keys[GLFW_KEY_RIGHT] >= GLFW_PRESS) {
            playerMove = "right";
            player->destination = player->position + glm::vec2(player->size.x, 0);
		}
	}
}

void Game::render(GLfloat interpolation) {
	if (this->state == GAME_ACTIVE) {
		level->draw(*renderer);
		player->draw(*renderer);
	} else if (this->state == GAME_GEN_LEVEL) {
		level->draw(*renderer);
		level->drawGenerating(*renderer);
	}
}
