
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"
#include "Player.h"
#include "TextRenderer.h"
#include "Menu.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <sstream>
#include <IrrKlang/irrKlang.h>

using namespace std;
// Game-related state data
SpriteRenderer* renderer;

GameLevel* level;
Player* player;

// Menus
Menu* mainMenu;
Menu* configMenu;
Menu* pauseMenu;
Menu* activeMenu;   // Pointer to active menu (Main, Config)

GLint Game::score = 0;
GLint Game::lifes = 3;
GLboolean keyActionPressed = false;
GLboolean keyPausePressed = false;
GLboolean keyPressedInMenu = false;

GLfloat rowsToClearFromTop = 0;   // Row to which to clear from top

GLint framesWaitingRespawn = 0;

// Game Constructor
Game::Game(GLFWwindow* window, GLuint width, GLuint height)
    : window(window), WIDTH(width), HEIGHT(height), time_step(0), _3DEnabled(false),
    musicEnabled(true), soundsEnabled(true) {}

// Game Destructor
Game::~Game() {
	delete renderer;
	delete level;
	delete mainMenu;
	delete configMenu;
}

void Game::init() {
	// Load shaders
	ResourceManager::loadShaderFromFile("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShaderFromFile("shaders/text.vs", "shaders/text.frag", nullptr, "text");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);

	// Sprite shader
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);

	// Text shader
    ResourceManager::getShader("text").use().setMatrix4("projection", projection);

	// Load textures
	ResourceManager::loadTexture("img/introPengo.png", GL_TRUE, "intro");
	ResourceManager::loadTexture("img/PengoMenuAnimation.png", GL_TRUE, "menuAnim");
	ResourceManager::loadTexture("img/walls.png", GL_TRUE, "walls");
	ResourceManager::loadTexture("img/diamond/diamond-shiny.png", GL_TRUE, "diamond-shiny");
	ResourceManager::loadTexture("img/blocks.png", GL_TRUE, "blocks");
	ResourceManager::loadTexture("img/creatures.png", GL_TRUE, "creatures");
	ResourceManager::loadTexture("img/indicators&eggs.png", GL_TRUE, "indicators-n-eggs");
	ResourceManager::loadTexture("img/pauseMenuBackground.png", GL_TRUE, "pause-background");

	// Set Render-specific contols
	Shader spriteShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(spriteShader, this->WIDTH, this->HEIGHT);

	Shader textShader = ResourceManager::getShader("text");
	ResourceManager::initTextRenderer(textShader, this->WIDTH, this->HEIGHT);

	level = new GameLevel();
	level->load("levels/level_testBonus.txt");

	player = level->pengo;
	player->movement = MOVE_DOWN;

	// Init music
	ResourceManager::initSound();

	// Create interface sprites
	lifesSprite = ResourceManager::getTexture("indicators-n-eggs");
	this->lifesSpriteFrame = SpriteFrame(this->lifesSprite.WIDTH, this->lifesSprite.HEIGHT, 160, 160, glm::vec2(0,0));
	this->eggsSpriteFrame = SpriteFrame(this->lifesSprite.WIDTH, this->lifesSprite.HEIGHT, 160, 160, glm::vec2(0,1));

	// Create intro
	introSprite = ResourceManager::getTexture("intro");
	this->introSpriteFrame = SpriteFrame(this->introSprite.WIDTH, this->introSprite.HEIGHT, 224, 288, glm::vec2(0,0));
	this->introSpriteFrame.readMap("img/introPengo.txt");

	// Main Menu
	mainMenu = new Menu(glm::vec2(5.0f, 11.5f));

	std::vector<Menu::MenuOption> mainMenuOptions;
	mainMenuOptions.push_back({"PLAY", glm::vec3(0.0f, 1.0f, 1.0f)});
	mainMenuOptions.push_back({"CONFIG", glm::vec3(0.0f, 1.0f, 1.0f)});
	mainMenuOptions.push_back({"EXIT", glm::vec3(0.0f, 1.0f, 1.0f)});

	mainMenu->setOptions(mainMenuOptions);

	// Main Menu Config options
	configMenu = new Menu(glm::vec2(5.0f, 11.5f));

	std::vector<Menu::MenuOption> configMenuOptions;
	configMenuOptions.push_back({"GRAPHICS  2D", glm::vec3(0.0f, 1.0f, 1.0f)});
	configMenuOptions.push_back({"MUSIC     ON", glm::vec3(0.0f, 1.0f, 1.0f)});
	configMenuOptions.push_back({"SOUNDS    ON", glm::vec3(0.0f, 1.0f, 1.0f)});
	configMenuOptions.push_back({"GO BACK", glm::vec3(0.0f, 1.0f, 1.0f)});

	configMenu->setOptions(configMenuOptions);

	// Title animation
	menuAnimSprite = ResourceManager::getTexture("menuAnim");
	this->menuAnimSpriteFrame = SpriteFrame(this->menuAnimSprite.WIDTH, this->menuAnimSprite.HEIGHT, 224, 85, glm::vec2(0,0));
	this->menuAnimSpriteFrame.readMap("img/PengoMenuAnimation.txt");

	// Ingame Menu
	pauseMenu = new Menu(glm::vec2(3.0f, 5.5f), renderer);

	std::vector<Menu::MenuOption> pauseMenuOptions;
	pauseMenuOptions.push_back({"CONTINUE", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"GRAPHICS  2D", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"MUSIC     ON", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"SOUNDS    ON", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"EXIT GAME", glm::vec3(0.0f, 1.0f, 1.0f)});

	pauseMenu->setOptions(pauseMenuOptions);

	activeMenu = mainMenu;
}

void Game::update() {
	time_step = (time_step+1)%(25 * 60 * 10); // Restart after 10 mins
    if (this->state == GAME_INTRO) {
    	introSpriteFrame.next(0.5);
    }

    else if (this->state == GAME_MENU) {
    	menuAnimSpriteFrame.next(0.5);
    }

    else if (this->state == GAME_ACTIVE) {
		if (level->state == LEVEL_PLAY || level->state == LEVEL_SHOWING_EGGS) {
			player->update();
		}
		level->update();
	}

    // Generate level
    else if(this->state == GAME_GEN_LEVEL) {
		if (level->generate()) {
            ResourceManager::soundEngine->stopAllSounds();
            ResourceManager::soundEngine->play2D("sounds/init_level.wav", false);
			this->state = GAME_START_LEVEL;
		}
	}
    else if(this->state == GAME_START_LEVEL) {
        if (!ResourceManager::soundEngine->isCurrentlyPlaying("sounds/init_level.wav")) {
			this->state = GAME_ACTIVE;
			ResourceManager::soundEngine->stopAllSounds();
            ResourceManager::soundEngine->play2D("sounds/level.wav", true);
			level->state = LEVEL_SHOWING_EGGS;
        }
    }
    else if(this->state == GAME_RESPAWN) {
        if (framesWaitingRespawn < 60) {
            framesWaitingRespawn++;
        }
        else {
            ResourceManager::soundEngine->play2D("sounds/init_level.wav", false);
            level->respawnPengo();
            player = level->pengo;
            this->state = GAME_START_LEVEL;
            framesWaitingRespawn = 0;
        }
    }
	if(level->state==LEVEL_BONUS) {
		if (time_step%4 == 0) {
			lifesSpriteFrame.setIndex(glm::vec2(((GLint) lifesSpriteFrame.getIndex().x + 1)%2,0));
		}
	}
	if(level->state==LEVEL_LOSE) {
		lifesSpriteFrame.setIndex(glm::vec2(2,0));
	}
}

static inline glm::vec2 nextPosRelative(Move m){
	switch(m){
		case MOVE_UP: return glm::vec2(0.0f,-1.0f);
		break;
		case MOVE_DOWN: return glm::vec2(0.0f,1.0f);
		break;
		case MOVE_RIGHT: return glm::vec2(1.0f,0.0f);
		break;
		case MOVE_LEFT: return glm::vec2(-1.0f,0.0f);
		break;
	}
}

void Game::proccessInput() {
	if (this->state == GAME_INTRO && this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS ) {
        this->state = GAME_MENU;
        keyPressedInMenu = true;
	}
	// IN MAIN MENU
	if (this->state == GAME_MENU) {
        if (this->keys[GLFW_KEY_DOWN] == GLFW_PRESS && !keyPressedInMenu) {
            activeMenu->nextOption();
            keyPressedInMenu = true;
        }
        else if(this->keys[GLFW_KEY_UP] == GLFW_PRESS && !keyPressedInMenu) {
            activeMenu->previousOption();
            keyPressedInMenu = true;
        }
        else if (this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS && !keyPressedInMenu) {
            keyPressedInMenu = true;
            if (activeMenu == mainMenu) {
                switch(mainMenu->getSelector()) {
                    case 0: // Play game
                        this->state = GAME_GEN_LEVEL;
                        ResourceManager::soundEngine->play2D("sounds/create_level.wav", true);
                    break;
                    case 1: // Enter config menu
                        activeMenu = configMenu;
                    break;
                    case 2: // Exit game
                        glfwSetWindowShouldClose(window, GL_TRUE);
                    break;
                }
            }
            else if(activeMenu == configMenu) {
                switch(configMenu->getSelector()) {
                    case 0: // GRAPHICS 2D/3D
                        _3DEnabled = !_3DEnabled;
                    break;
                    case 1: // MUSIC ON/OFF
                        if (musicEnabled) {
                            configMenu->options[1].text = "MUSIC     OFF";
                            musicEnabled = false;
                        }
                        else {
                            configMenu->options[1].text = "MUSIC     ON";
                            musicEnabled = true;
                        }
                    break;
                    case 2: // SOUNDS ON/OFF
                        if (soundsEnabled) {
                            configMenu->options[2].text = "SOUNDS    OFF";
                            soundsEnabled = false;
                        }
                        else {
                            configMenu->options[2].text = "SOUNDS    ON";
                            soundsEnabled = true;
                        }
                    break;
                    case 3: // GO BACK TO MAIN MENU
                        activeMenu = mainMenu;
                    break;
                }
            }
        }
        else if (this->keys[GLFW_KEY_DOWN] == GLFW_RELEASE && this->keys[GLFW_KEY_UP] == GLFW_RELEASE
                 && this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_RELEASE){
            keyPressedInMenu = false;
        }
	}

    // IN PAUSE MENU
    else if (this->state == GAME_PAUSE_MENU) {
        if (this->keys[GLFW_KEY_DOWN] == GLFW_PRESS && !keyPressedInMenu) {
            pauseMenu->nextOption();
            keyPressedInMenu = true;
        }
        else if(this->keys[GLFW_KEY_UP] == GLFW_PRESS && !keyPressedInMenu) {
            pauseMenu->previousOption();
            keyPressedInMenu = true;
        }
        else if (this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS && !keyPressedInMenu) {
            keyPressedInMenu = true;
            switch(pauseMenu->getSelector()) {
                case 0: // CONTINUE GAME
                    this->state = GAME_ACTIVE;
                    ResourceManager::soundEngine->setAllSoundsPaused(false);
                    keyActionPressed = true;
                break;
                case 1: // GRAPHICS 2D/3D
                    _3DEnabled = !_3DEnabled;
                break;
                case 2: // MUSIC ON/OFF
                    if (musicEnabled) {
                        pauseMenu->options[2].text = "MUSIC     OFF";
                        musicEnabled = false;
                    }
                    else {
                        pauseMenu->options[2].text = "MUSIC     ON";
                        musicEnabled = true;
                    }
                break;
                case 3: // SOUNDS ON/OFF
                    if (soundsEnabled) {
                        pauseMenu->options[3].text = "SOUNDS    OFF";
                        soundsEnabled = false;
                    }
                    else {
                        pauseMenu->options[3].text = "SOUNDS    ON";
                        soundsEnabled = true;
                    }
                break;
                case 4: // GO BACK TO MAIN MENU
                    delete level;
                    level = new GameLevel();
                    level->load("levels/level_testBonus.txt");
                    player = level->pengo;
                    Game::lifes = 3;
                    Game::score = 0;
                    activeMenu = mainMenu;
                    this->state = GAME_MENU;
                break;
            }
        }
        else if (this->keys[GLFW_KEY_DOWN] == GLFW_RELEASE && this->keys[GLFW_KEY_UP] == GLFW_RELEASE
                 && this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_RELEASE){
            keyPressedInMenu = false;
        }
    }
	// IN GAME
	else if (this->state == GAME_ACTIVE) {
        if(this->keys[GLFW_KEY_ESCAPE] == GLFW_PRESS && !keyPausePressed) {
            keyPausePressed = true;
            ResourceManager::soundEngine->setAllSoundsPaused(true);
            this->state = GAME_PAUSE_MENU;
        }
        else if (this->keys[GLFW_KEY_ESCAPE] == GLFW_RELEASE) {
            keyPausePressed = false;
        }
		if(this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS && player->state==STOPPED && !keyActionPressed) {
			// Look in front of Pengo
			glm::vec2 npr = nextPosRelative(player->movement);
			if (level->checkCollision(player->position + npr)) {
				// If iceblock -> Slide or disintegrate
				Iceblock* block = dynamic_cast< Iceblock* >(level->getObjFromPosition(player->position + npr));
				if (block!=nullptr){
					if (!level->checkCollision(player->position + (npr + npr))) {
						// Slide
						block->slide(player->movement,level);
	    				ResourceManager::soundEngine->play2D("sounds/push-ice-block.wav", false);
						player->state = PUSHING;
					} else {
						block->disintegrate(level, true);
	    				ResourceManager::soundEngine->play2D("sounds/ice-block-destroyed.wav", false);
						player->state = DESTROYING;
					}
				}

				// If diamondblock -> Slide or nothing
				Diamondblock* dblock = dynamic_cast< Diamondblock* >(level->getObjFromPosition(player->position + npr));
				if (dblock!=nullptr){
					if (!level->checkCollision(player->position + (npr + npr))) {
						// Slide
						dblock->slide(player->movement,level);
	    				ResourceManager::soundEngine->play2D("sounds/push-ice-block.wav", false);
						player->state = PUSHING;
					}
				}
				// If walls -> shake
			    if ((player->position + npr).y>=17) {
	    			ResourceManager::soundEngine->play2D("sounds/push-outside-walls.wav", false);
			        for (GLuint i = 0; i < level->wallS.size(); i++) {
			        	level->wallS[i].shake(1,i%2);
						player->state = DESTROYING;
			        }
			    } else if((player->position + npr).y < 2) {
	    			ResourceManager::soundEngine->play2D("sounds/push-outside-walls.wav", false);
			        for (GLuint i = 0; i < level->wallN.size(); i++) {
			        	level->wallN[i].shake(1,i%2);
						player->state = DESTROYING;
			        }
			    } else if((player->position + npr).x>=13.5f) {
	    			ResourceManager::soundEngine->play2D("sounds/push-outside-walls.wav", false);
			        for (GLuint i = 0; i < level->wallE.size(); i++) {
			        	level->wallE[i].shake(0,i%2);
						player->state = DESTROYING;
			        }
			    } else if ((player->position + npr).x < 0.5f) {
	    			ResourceManager::soundEngine->play2D("sounds/push-outside-walls.wav", false);
			        for (GLuint i = 0; i < level->wallW.size(); i++) {
			        	level->wallW[i].shake(0,i%2);
						player->state = DESTROYING;
			        }
			    }
			}
		}
		if(this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS && player->state!=MOVING) {
			keyActionPressed = true;
		}
		if (this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_RELEASE) {
			keyActionPressed = false;
		}
		// Move playerboard
		glm::vec2 newPos;
		if (this->keys[GLFW_KEY_UP] >= GLFW_PRESS) {
			if (player->state==STOPPED) {//!player->isMoving
	        	player->movement = MOVE_UP;
	            newPos = player->position + glm::vec2(0, -1);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}

		if (this->keys[GLFW_KEY_DOWN] >= GLFW_PRESS) {
			if (player->state==STOPPED) {
	        	player->movement = MOVE_DOWN;
	            newPos = player->position + glm::vec2(0, 1);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}

		if (this->keys[GLFW_KEY_LEFT] >= GLFW_PRESS) {
			if (player->state==STOPPED) {
	        	player->movement = MOVE_LEFT;
	            newPos = player->position + glm::vec2(-1, 0);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}

		if (this->keys[GLFW_KEY_RIGHT] >= GLFW_PRESS) {
			if (player->state==STOPPED) {
	        	player->movement = MOVE_RIGHT;
	            newPos = player->position + glm::vec2(1, 0);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}
	}
}

void Game::render(GLfloat interpolation) {
    if (this->state == GAME_INTRO) {
    	renderer->drawSprite(this->introSprite, glm::vec2(0,0), glm::vec2(14,18), (this->introSpriteFrame));//WIDTH, HEIGHT
    } else {
	    ResourceManager::textRenderer->renderText("1P", glm::vec2(0.5,0), 0.5f, glm::vec3(0.0f, 1.0f, 1.0f));
		ostringstream strs;
		GLint numDigits = 1;
		GLint tmpScore = score;
		while (tmpScore>=10) {
			tmpScore = tmpScore/10;
			numDigits++;
		}
		while(numDigits<10) {
			strs << " ";
			numDigits++;
		}
		strs << score;
		string str = strs.str();
	    ResourceManager::textRenderer->renderText(str, glm::vec2(1.5,0), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	    ResourceManager::textRenderer->renderText("HI", glm::vec2(7.5,0), 0.5f, glm::vec3(0.0f, 1.0f, 1.0f));
	    ResourceManager::textRenderer->renderText("20000", glm::vec2(11,0), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	    // Draw lifes
	    for(int i = 0; i<lifes-1; i++) {
			renderer->drawSprite(this->lifesSprite, glm::vec2(i,0.5), glm::vec2(1,1), this->lifesSpriteFrame);
	    }

	    // Draw Eggs
	    for(int i = 0; i<level->liveEnemies-level->deadEnemies; i++) {
			renderer->drawSprite(this->lifesSprite, glm::vec2(6.5f+i*0.5f, 1), glm::vec2(0.5f,0.5f), this->eggsSpriteFrame);
	    }
    }

    if (this->state == GAME_ACTIVE && level->state != LEVEL_BONUS && level->state != LEVEL_LOSE && level->state != LEVEL_LOSE2&& level->state != LEVEL_TMP) {
	    player->move(player->movement, interpolation);
    	level->moveEnemies(interpolation);
	    level->moveBlocks(interpolation);
	    level->destroyBlocks(interpolation);

	    for (auto &i : level->enemies) {
	        if (i != nullptr) {
	            if (player->overlaps(i)){
	            	if (i->state == NUMB) {
		                ResourceManager::soundEngine->play2D("sounds/touch-snow-bee.wav", false);
                    	level->floatingTexts.push_back(new FloatingText(i->position + glm::vec2(0.0f,0.3f), "100", 50, 0.33, glm::vec3(1.0f,1.0f,1.0f)));
		                level->liveEnemies--;
		                level->deadEnemies++;
		                score += 100;
		                i = nullptr;
	            	} else {
	            		level->state = LEVEL_LOSE;
            			ResourceManager::soundEngine->stopAllSounds();
                		player->changeIndexFrame(glm::vec2(0, 2));
	            	}
	            }
	        }
	    }
    }
    if (this->state == GAME_ACTIVE) {
        if (level->state == LEVEL_TMP) {
            if (rowsToClearFromTop > 17.0f) {
                this->state = GAME_RESPAWN;
                rowsToClearFromTop = 0;
            }
            else {
                level->clearFromTop(*renderer, rowsToClearFromTop);
                rowsToClearFromTop += interpolation;
            }
        }
        else {
            level->draw(*renderer);
            player->draw(*renderer);
        }
    }
    else if (this->state == GAME_START_LEVEL) {
		level->draw(*renderer);
		player->draw(*renderer);
	}
	else if (this->state == GAME_GEN_LEVEL) {
		level->draw(*renderer);
		level->drawGenerating(*renderer);
	}
	else if (this->state == GAME_MENU) {
		renderer->drawSprite(this->menuAnimSprite, glm::vec2(0,0.5f), glm::vec2(14,5), (this->menuAnimSpriteFrame));//WIDTH, HEIGHT 5.3125f
        activeMenu->drawMenu();
	}
	else if (this->state == GAME_PAUSE_MENU) {
        level->draw(*renderer);
        player->draw(*renderer);
        pauseMenu->drawMenu();
	}
	else if (this->state == GAME_RESPAWN) {
        ResourceManager::textRenderer->renderText("GET READY", glm::vec2(5,6), 0.5f, glm::vec3(1,1,1));
	}
}
