
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
GLint Game::lifes = 2;
GLint Game::timeLevel = 0;
GLint timeLevelStep = 0;

GLboolean Game::musicEnabled = true;
GLboolean Game::soundsEnabled = true;
GLboolean Game::_3DEnabled = false;

GLboolean keyActionPressed = false;
GLboolean keyPausePressed = false;
GLboolean keyCheatPressed = false;
GLboolean keyPressedInMenu = false;
GLboolean keyPressedInRecords = false;

GLfloat rowsToClearFromTop = 0;   // Row to which to clear from top

GLint framesWaitingRespawn = 0;

GLint framesShowingGameOver = 0;

GLint framesOnGameWin = 0;
GLint framesOnBonusTime = 0;
GLboolean endRanking = false;
GLboolean colRankingName = false;
GLint rankingSetChar = 0;
GLint rankingNewPos = 5;

// Cheat list
GLboolean Game::cheat_Invincible = false;
GLboolean Game::cheat_InfiniteLifes = false;

// Game Constructor
Game::Game(GLFWwindow* window, GLuint width, GLuint height)
    : window(window), WIDTH(width), HEIGHT(height), time_step(0), maxEggsInLevel(6) {}

// Game Destructor
Game::~Game() {
	delete renderer;
	delete level;
	delete mainMenu;
	delete configMenu;
}

void Game::readHighScores() {
    std::ifstream infile("records.txt");
    GLint sc;
    std::string scName;
    while (infile >> sc >> scName) {
        highScores.push_back(sc);
        highScoresNames.push_back(scName);
    }
    infile.close();
}

void Game::writeHighScores(){
    std::ofstream outfile("records.txt");
    for(int i = 0; i < 5; i++) {
        outfile << highScores[i] << " " << highScoresNames[i] << std::endl;
    }
    outfile.close();
}

void Game::init() {
    playerName[0] = 0;
    playerName[1] = 0;
    playerName[2] = 0;

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

	// Store filenames of all levels and load a random level
	allLevels.push_back("levels/level1.txt");
	allLevels.push_back("levels/level_testBonus.txt");
	allLevels.push_back("levels/level2.txt");
	allLevels.push_back("levels/level3.txt");

	levelsToPlay = std::vector<std::string>(allLevels);

	level = new GameLevel(maxEggsInLevel);
	GLint r = rand() % allLevels.size();
	level->load(levelsToPlay[r]);
	levelsToPlay.erase(levelsToPlay.begin() + r);

	player = level->pengo;
	player->movement = MOVE_DOWN;

    // Load scores
    readHighScores();

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
	configMenu = new Menu(glm::vec2(4.0f, 11.5f));

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
    	if(introSpriteFrame.getFrameIterator() < introSpriteFrame.getNumFrames()-1) {
    		introSpriteFrame.next(0.5);
    	} else {
        	this->state = GAME_MENU;
    	}
    }

    else if (this->state == GAME_MENU) {
    	if(menuAnimSpriteFrame.getFrameIterator() < menuAnimSpriteFrame.getNumFrames()-1){
    		menuAnimSpriteFrame.next(0.5);
    	}
    }

    else if (this->state == GAME_ACTIVE) {
		if (level->state == LEVEL_PLAY || level->state == LEVEL_SHOWING_EGGS) {
			player->update();
            if (timeLevelStep == 25) {
                timeLevelStep = 0;
                timeLevel++;
            } else {
                timeLevelStep++;
            }
		}
		else if (level->state == LEVEL_TMP) {
            if (rowsToClearFromTop > 17.0f) {
                if (lifes > 0) {
                    if (!cheat_InfiniteLifes) {
                        lifes--;
                    }
                    this->state = GAME_RESPAWN;
                }
                else {
                    rowsToClearFromTop = 0;
                    this->state = GAME_OVER;
                }

                lifesSpriteFrame.setIndex(glm::vec2(0,0));
            }
        }
        else if (level->state == LEVEL_WIN) {
            if (framesOnGameWin >= 50) {
                ResourceManager::musicEngine->stopAllSounds();
                ResourceManager::musicEngine->play2D("sounds/act-clear.wav", false);
                player->movement = (player->position.x >= 7) ? MOVE_LEFT : MOVE_RIGHT;
                player->destination = (player->position.x >= 7) ? glm::vec2(-1, player->position.y) : glm::vec2(15, player->position.y);
                player->state = MOVING;
                framesOnGameWin = 0;
                this->state = GAME_WIN;
            }
            else {
                player->update();
                framesOnGameWin++;
            }
        }
		level->update();
	}

    // Generate level
    else if(this->state == GAME_GEN_LEVEL) {
		if (level->generate()) {
            ResourceManager::musicEngine->stopAllSounds();
            ResourceManager::musicEngine->play2D("sounds/init_level.wav", false);
			this->state = GAME_START_LEVEL;
		}
	}
    else if(this->state == GAME_START_LEVEL) {
        if (!ResourceManager::musicEngine->isCurrentlyPlaying("sounds/init_level.wav")) {
			this->state = GAME_ACTIVE;
			ResourceManager::musicEngine->stopAllSounds();
            ResourceManager::musicEngine->play2D("sounds/level.wav", true);
			level->state = LEVEL_SHOWING_EGGS;
        }
    }
    else if(this->state == GAME_RESPAWN) {
        if (framesWaitingRespawn == 50) {
            level->respawnBlocks();
        }
        if (framesWaitingRespawn < 60) {
            framesWaitingRespawn++;
        }
        else {
            if (rowsToClearFromTop <= 0.0f) {
                ResourceManager::musicEngine->play2D("sounds/init_level.wav", false);
                level->respawnPengo();
                player = level->pengo;
                level->respawnEnemiesAtCorners();
                this->state = GAME_START_LEVEL;
                framesWaitingRespawn = 0;
            }
        }
    }
    else if(this->state == GAME_OVER) {
        if (framesShowingGameOver < 50) {
            framesShowingGameOver++;
        }
        else {
            this->state = GAME_RECORDS;
            if (score>=highScores[4]) {
                ResourceManager::musicEngine->stopAllSounds();
                ResourceManager::musicEngine->play2D("sounds/name-entry.wav", true);
                rankingNewPos = 4;

                // Found new record position
                GLboolean found = false;
                for(int i = 0; i < 4 && !found; i++) {
                    if (score >= highScores[i]){
                        rankingNewPos = i;
                        found = true;
                    }
                }

                // Place the new record
                for (int i = 4; i>rankingNewPos; i--){
                    highScores[i] = highScores[i-1];
                    highScoresNames[i] = highScoresNames[i-1];
                }
                highScores[rankingNewPos] = score;
                highScoresNames[rankingNewPos] = "A";

                endRanking = false;
            } else {
                endRanking = true;
            }
        }
    }
    else if(this->state == GAME_RECORDS) {
        if (endRanking)  {
            ResourceManager::musicEngine->stopAllSounds();
            endRanking = false;
            delete level;
            maxEggsInLevel = 6;
            level = new GameLevel(maxEggsInLevel);
            levelsToPlay = std::vector<std::string>(allLevels);

            // Load random level
            GLint r = rand() % levelsToPlay.size();
            level->load(levelsToPlay[r]);
            levelsToPlay.erase(levelsToPlay.begin() + r);
            player = level->pengo;

            Game::lifes = 2;
            Game::score = 0;
            activeMenu = mainMenu;
            framesShowingGameOver = 0;
            this->state = GAME_MENU;
        } else {
            if(time_step%4 == 0){
                colRankingName = !colRankingName;
            }
        }
    }
    else if(this->state == GAME_WIN) {
        if (ResourceManager::musicEngine->isCurrentlyPlaying("sounds/act-clear.wav")) {
            player->update();
        }
        else {
            this->state = GAME_BONUSTIME;
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
    if(this->state == GAME_BONUSTIME) {
        framesOnBonusTime++;
        if(framesOnBonusTime<240) {
            if(framesOnBonusTime==30) {
                // Give time bonus
                if (timeLevel<20) {
                    score += 5000;
                } else if(timeLevel < 30) {
                    score += 2000;
                } else if(timeLevel < 40) {
                    score += 1000;
                } else if(timeLevel < 50) {
                    score += 500;
                } else if(timeLevel < 60) {
                    score += 10;
                }
            }

            if (framesOnBonusTime == 50){
                ResourceManager::musicEngine->stopAllSounds();
                ResourceManager::musicEngine->play2D("sounds/intermission.wav", true);
            }
        } else {
            ResourceManager::musicEngine->stopAllSounds();
            framesOnBonusTime = 0;
            timeLevel = 0;

            // Change level
            delete level;
            if (maxEggsInLevel < 12) {
                maxEggsInLevel++;
            }
            level = new GameLevel(maxEggsInLevel);
            if (levelsToPlay.size() == 0) {
                levelsToPlay = std::vector<std::string>(allLevels);
            }

            // Load random level
            GLint r = rand() % levelsToPlay.size();
            level->load(levelsToPlay[r]);
            levelsToPlay.erase(levelsToPlay.begin() + r);
            player = level->pengo;

            ResourceManager::musicEngine->play2D("sounds/create_level.wav", true);
            this->state = GAME_GEN_LEVEL;
        }
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

static inline string toString(int v) {
    ostringstream strs;
    strs << v;
    return strs.str();
}

static inline string toStringFill(int v, int size) {
    ostringstream strs;
    GLint numDigits = 1;
    GLint tmpV = v;
    while (tmpV>=10) {
        tmpV = tmpV/10;
        numDigits++;
    }
    while(numDigits<size) {
        strs << " ";
        numDigits++;
    }

    strs << v;
    return strs.str();
}

void Game::proccessInput() {
    if (this->keys[GLFW_KEY_C] == GLFW_PRESS && !keyCheatPressed) {
        keyCheatPressed = true;
        // READ CHEATS
        string cheat;
        cin >> cheat;
        if (cheat.compare("Zodd")==0) {
            cheat_Invincible = !cheat_Invincible;
            cout << "The immortal" << endl;
        } else if (cheat.compare("Rei")==0) {
            cheat_InfiniteLifes = !cheat_InfiniteLifes;
            cout << "If I die, I can be replaced" << endl;
        } else if (cheat.compare("ImPro")==0) {
            level->state = LEVEL_WIN;
            cout << "Yes... Congratulations..." << endl;
        } else {
            cout << "Unknow cheat" << endl;
        }
    } else if (this->keys[GLFW_KEY_C] == GLFW_RELEASE) {
        keyCheatPressed = false;
    }

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
                        ResourceManager::musicEngine->play2D("sounds/create_level.wav", true);
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
                            pauseMenu->options[2].text = "MUSIC     OFF";
                            ResourceManager::musicEngine->setSoundVolume(0);
                            musicEnabled = false;
                        }
                        else {
                            configMenu->options[1].text = "MUSIC     ON";
                            pauseMenu->options[2].text = "MUSIC     ON";
                            ResourceManager::musicEngine->setSoundVolume(1);
                            musicEnabled = true;
                        }
                    break;
                    case 2: // SOUNDS ON/OFF
                        if (soundsEnabled) {
                            configMenu->options[2].text = "SOUNDS    OFF";
                            pauseMenu->options[3].text = "SOUNDS    OFF";
                            ResourceManager::soundEngine->setSoundVolume(0);
                            soundsEnabled = false;
                        }
                        else {
                            configMenu->options[2].text = "SOUNDS    ON";
                            pauseMenu->options[3].text = "SOUNDS    ON";
                            ResourceManager::soundEngine->setSoundVolume(1);
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
                    ResourceManager::musicEngine->setAllSoundsPaused(false);
                    ResourceManager::soundEngine->setAllSoundsPaused(false);
                    keyActionPressed = true;
                break;
                case 1: // GRAPHICS 2D/3D
                    _3DEnabled = !_3DEnabled;
                break;
                case 2: // MUSIC ON/OFF
                    if (musicEnabled) {
                        configMenu->options[1].text = "MUSIC     OFF";
                        pauseMenu->options[2].text = "MUSIC     OFF";
                        ResourceManager::musicEngine->setSoundVolume(0);
                        musicEnabled = false;
                    }
                    else {
                        configMenu->options[1].text = "MUSIC     ON";
                        pauseMenu->options[2].text = "MUSIC     ON";
                        ResourceManager::musicEngine->setSoundVolume(1);
                        musicEnabled = true;
                    }
                break;
                case 3: // SOUNDS ON/OFF
                    if (soundsEnabled) {
                        configMenu->options[2].text = "SOUNDS    OFF";
                        pauseMenu->options[3].text = "SOUNDS    OFF";
                        ResourceManager::soundEngine->setSoundVolume(0);
                        soundsEnabled = false;
                    }
                    else {
                        configMenu->options[2].text = "SOUNDS    ON";
                        pauseMenu->options[3].text = "SOUNDS    ON";
                        ResourceManager::soundEngine->setSoundVolume(1);
                        soundsEnabled = true;
                    }
                break;
                case 4: // GO BACK TO MAIN MENU
                    delete level;
                    maxEggsInLevel = 6;
                    level = new GameLevel(maxEggsInLevel);
                    levelsToPlay = std::vector<std::string>(allLevels);

                    // Load random level
                    GLint r = rand() % levelsToPlay.size();
                    level->load(levelsToPlay[r]);
                    levelsToPlay.erase(levelsToPlay.begin() + r);

                    player = level->pengo;
                    Game::lifes = 2;
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
            ResourceManager::musicEngine->setAllSoundsPaused(true);
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
				if (block!=nullptr && block->state == STOPPED){
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
				if (dblock!=nullptr && dblock->state == STOPPED){
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


    // IN RECORDS
    else if (this->state == GAME_RECORDS) {
        if(this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS  && !keyPressedInRecords) {
            keyPressedInRecords = true;
            if (rankingSetChar==2) {
                endRanking = true;
                writeHighScores();
            } else {
                rankingSetChar = (rankingSetChar + 1) % 3;
                highScoresNames[rankingNewPos] = "";
                for(int i = 0; i <= rankingSetChar; i++){
                    highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
                }
            }
        }
        if(this->keys[GLFW_KEY_RIGHT] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            rankingSetChar = (rankingSetChar + 1) % 3;
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }
        else if(this->keys[GLFW_KEY_LEFT] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            rankingSetChar = (rankingSetChar - 1 + 3) % 3;
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }
        else if(this->keys[GLFW_KEY_UP] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            playerName[rankingSetChar] = (playerName[rankingSetChar] + 1) % ('Z' - 'A' + 1);
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }
        else if(this->keys[GLFW_KEY_DOWN] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            playerName[rankingSetChar] = (playerName[rankingSetChar] - 1 + ('Z' - 'A' + 1)) % ('Z' - 'A' + 1);
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }

        if (this->keys[GLFW_KEY_DOWN] == GLFW_RELEASE && this->keys[GLFW_KEY_UP] == GLFW_RELEASE
                && this->keys[GLFW_KEY_RIGHT] == GLFW_RELEASE && this->keys[GLFW_KEY_LEFT] == GLFW_RELEASE
                 && this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_RELEASE){
            keyPressedInRecords = false;
        }
    }
}

void Game::render(GLfloat interpolation) {
    if (this->state == GAME_INTRO) {
    	renderer->drawSprite(this->introSprite, glm::vec2(0,0), glm::vec2(14,18), (this->introSpriteFrame));//WIDTH, HEIGHT
    } else {
	    ResourceManager::textRenderer->renderText("1P", glm::vec2(0.5,0), 0.5f, glm::vec3(0.0f, 1.0f, 1.0f));
	    ResourceManager::textRenderer->renderText(toStringFill(score,10), glm::vec2(1.5,0), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	    ResourceManager::textRenderer->renderText("HI", glm::vec2(7.5,0), 0.5f, glm::vec3(0.0f, 1.0f, 1.0f));
	    ResourceManager::textRenderer->renderText(toStringFill(highScores[0],10), glm::vec2(8.5,0), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	    // Draw lifes
	    for(int i = 0; i<lifes; i++) {
			renderer->drawSprite(this->lifesSprite, glm::vec2(i,0.5), glm::vec2(1,1), this->lifesSpriteFrame);
	    }

	    // Draw Eggs
	    for(int i = 0; i<level->numEggs-level->deadEnemies-level->liveEnemies; i++) {
			renderer->drawSprite(this->lifesSprite, glm::vec2(6.5f+i*0.5f, 1), glm::vec2(0.5f,0.5f), this->eggsSpriteFrame);
	    }
	    if (this->state != GAME_MENU && this->state != GAME_PAUSE_MENU) {
            ResourceManager::textRenderer->renderText("CTRL:PUSH    ARROWS:MOVE     ESC:PAUSE", glm::vec2(0,17.6f), 0.3f, glm::vec3(1,1,1));
	    }
    }

    if (this->state == GAME_ACTIVE && level->state != LEVEL_BONUS && level->state != LEVEL_LOSE && level->state != LEVEL_LOSE2&& level->state != LEVEL_TMP) {
	    player->move(player->movement, interpolation);
    	level->moveEnemies(interpolation);
	    level->moveBlocks(interpolation);
	    level->destroyBlocks(interpolation);

        int nKills = 0;
        glm::vec2 killPos;
	    for (auto &i : level->enemies) {
	        if (i != nullptr) {
	            if (player->overlaps(i)){
	            	if (i->state == NUMB) {
                        i->state = DEAD;
                        killPos = i->position;
                        i = nullptr;
                        nKills++;
	            	} else if (!cheat_Invincible) {
	            		level->state = LEVEL_LOSE;
            			ResourceManager::soundEngine->stopAllSounds();
            			ResourceManager::musicEngine->stopAllSounds();
                		player->changeIndexFrame(glm::vec2(0, 2));
	            	}
	            }
	        }
	    }
        if (nKills>0) {
            ResourceManager::soundEngine->play2D("sounds/touch-snow-bee.wav", false);
            level->floatingTexts.push_back(new FloatingText(killPos + glm::vec2(0.0f,0.3f), toString(nKills*100), 50, 0.33, glm::vec3(1.0f,1.0f,1.0f)));
            level->liveEnemies-=nKills;
            level->deadEnemies+=nKills;
            score += nKills*100;
        }
    }
    if (this->state == GAME_ACTIVE) {
        if (level->state == LEVEL_TMP) {
            level->clearFromTop(*renderer, rowsToClearFromTop);
            rowsToClearFromTop += interpolation;
        }
        else {
            level->draw(*renderer);
        }
    }
    else if (this->state == GAME_START_LEVEL) {
		level->draw(*renderer);
	}
	else if (this->state == GAME_GEN_LEVEL) {
		level->drawGenerating(*renderer);
	}
	else if (this->state == GAME_MENU) {
		renderer->drawSprite(this->menuAnimSprite, glm::vec2(0,0.5f), glm::vec2(14,5), (this->menuAnimSpriteFrame));//WIDTH, HEIGHT 5.3125f
        activeMenu->drawMenu();
        ResourceManager::textRenderer->renderText("CTRL: SELECT    UP/DOWN ARROW: MOVE", glm::vec2(0,17.6f), 0.3f, glm::vec3(1,1,1));
	}
	else if (this->state == GAME_PAUSE_MENU) {
        level->draw(*renderer);
        pauseMenu->drawMenu();
        ResourceManager::textRenderer->renderText("CTRL: SELECT    UP/DOWN ARROW: MOVE", glm::vec2(0,17.6f), 0.3f, glm::vec3(1,1,1));
	}
	else if (this->state == GAME_RESPAWN) {
        if (framesWaitingRespawn < 60) {
            ResourceManager::textRenderer->renderText("GET READY", glm::vec2(5,6), 0.5f, glm::vec3(1,1,1));
        }
        else {
            if (rowsToClearFromTop > 0.0f) {
                level->clearFromTop(*renderer, rowsToClearFromTop);
                rowsToClearFromTop -= interpolation;
            }
        }
	}
	else if (this->state == GAME_OVER) {
        ResourceManager::textRenderer->renderText("GAME OVER", glm::vec2(5,6), 0.5f, glm::vec3(1,1,1));
        ResourceManager::textRenderer->renderText("THANKS FOR PLAYING", glm::vec2(3,7), 0.5f, glm::vec3(1,1,1));
	}
	else if (this->state == GAME_WIN) {
        //ResourceManager::textRenderer->renderText("GET READY", glm::vec2(5,6), 0.5f, glm::vec3(1,1,1));
        player->move(player->movement, interpolation);
        player->draw(*renderer);
	}
    else if (this->state == GAME_BONUSTIME) {
        //ResourceManager::textRenderer->renderText("GAME TIME  " + toString(timeLevel) + " SEC.", glm::vec2(1,2.5f), 0.5f, glm::vec3(1,1,0));
        GLint minutes = timeLevel/60;
        GLint secs = timeLevel%60;

        if(framesOnBonusTime>15) {
            if (minutes > 0){
                if (minutes >= 10){
                    ResourceManager::textRenderer->renderText("          " + toString(minutes), glm::vec2(1,2.5f), 0.5f, glm::vec3(1,1,1));
                } else {
                    ResourceManager::textRenderer->renderText("           " + toString(minutes), glm::vec2(1,2.5f), 0.5f, glm::vec3(1,1,1));
                }
            }
            if (secs >= 10){
                ResourceManager::textRenderer->renderText("                 " + toString(secs), glm::vec2(1,2.5f), 0.5f, glm::vec3(1,1,1));
            } else {
                ResourceManager::textRenderer->renderText("                  " + toString(secs), glm::vec2(1,2.5f), 0.5f, glm::vec3(1,1,1));
            }
        }
        ResourceManager::textRenderer->renderText("GAME TIME    MIN.   SEC.", glm::vec2(1,2.5f), 0.5f, glm::vec3(1,1,0));
        glm::vec3 colBonus[6];
        for (int i = 0; i < 6; i++) {
            colBonus[i] = glm::vec3(0,1,1);
        }

        if(framesOnBonusTime>30) {
            if (timeLevel<20) {
                colBonus[0] = glm::vec3(1,1,0);
            } else if(timeLevel < 30) {
                colBonus[1] = glm::vec3(1,1,0);
            } else if(timeLevel < 40) {
                colBonus[2] = glm::vec3(1,1,0);
            } else if(timeLevel < 50) {
                colBonus[3] = glm::vec3(1,1,0);
            } else if(timeLevel < 60) {
                colBonus[4] = glm::vec3(1,1,0);
            } else {
                colBonus[5] = glm::vec3(1,1,0);
            }
        }
        ResourceManager::textRenderer->renderText("FROM 00 TO 19 .5000 PTS.", glm::vec2(1,4), 0.5f, colBonus[0]);
        ResourceManager::textRenderer->renderText("FROM 20 TO 29 .2000 PTS.", glm::vec2(1,5), 0.5f, colBonus[1]);
        ResourceManager::textRenderer->renderText("FROM 30 TO 39 .1000 PTS.", glm::vec2(1,6), 0.5f, colBonus[2]);
        ResourceManager::textRenderer->renderText("FROM 40 TO 49 ..500 PTS.", glm::vec2(1,7), 0.5f, colBonus[3]);
        ResourceManager::textRenderer->renderText("FROM 50 TO 59 ...10 PTS.", glm::vec2(1,8), 0.5f, colBonus[4]);
        ResourceManager::textRenderer->renderText("60 AND OVER    NO BONUS.", glm::vec2(1,9), 0.5f, colBonus[5]);
    }
    else if (this->state == GAME_RECORDS) {
        ResourceManager::textRenderer->renderText("ENTER YOUR INITIALS", glm::vec2(3,2.5), 0.5f, glm::vec3(1,1,0));
        ResourceManager::textRenderer->renderText("SCORE    NAME", glm::vec2(6,3.5), 0.5f, glm::vec3(0, 1, 0));
        ResourceManager::textRenderer->renderText(toStringFill(score,6), glm::vec2(5.5f,4.5), 0.5f, glm::vec3(1, 1, 1));
        std::string empty = "";
        glm::vec3 colSelected(1,1,1);
        if (colRankingName) {
            colSelected = glm::vec3(1,0,0);
        }
        char text1 = ((char) playerName[0] + 'A');
        char text2 = ((char) playerName[1] + 'A');
        char text3 = ((char) playerName[2] + 'A');
        if(rankingSetChar==0) {
            ResourceManager::textRenderer->renderText(empty + text1, glm::vec2(11,4.5), 0.5f, colSelected);
        } else {
            ResourceManager::textRenderer->renderText(empty + text1, glm::vec2(11,4.5), 0.5f, glm::vec3(1, 1, 0));
        }
        if(rankingSetChar==1) {
            ResourceManager::textRenderer->renderText(empty + text2, glm::vec2(11.5,4.5), 0.5f, colSelected);
        } else {
            ResourceManager::textRenderer->renderText(empty + text2, glm::vec2(11.5,4.5), 0.5f, glm::vec3(1, 1, 0));
        }
        if(rankingSetChar==2) {
            ResourceManager::textRenderer->renderText(empty + text3, glm::vec2(12,4.5), 0.5f, colSelected);
        } else {
            ResourceManager::textRenderer->renderText(empty + text3, glm::vec2(12,4.5), 0.5f, glm::vec3(1, 1, 0));
        }
        ResourceManager::textRenderer->renderText("SCORE    NAME", glm::vec2(6,6.5), 0.5f, glm::vec3(1, 0.7019, 0.8431f));
        ResourceManager::textRenderer->renderText("1ST " + toStringFill(highScores[0],6) + "     " + highScoresNames[0], 
            glm::vec2(3.5,7.5), 0.5f, glm::vec3(0, 1, 1));
        ResourceManager::textRenderer->renderText("2ND " + toStringFill(highScores[1],6) + "     " + highScoresNames[1], 
            glm::vec2(3.5,8.5), 0.5f, glm::vec3(0, 1, 1));
        ResourceManager::textRenderer->renderText("3RD " + toStringFill(highScores[2],6) + "     " + highScoresNames[2], 
            glm::vec2(3.5,9.5), 0.5f, glm::vec3(0, 1, 1));
        ResourceManager::textRenderer->renderText("4TH " + toStringFill(highScores[3],6) + "     " + highScoresNames[3], 
            glm::vec2(3.5,10.5), 0.5f, glm::vec3(0, 1, 1));
        ResourceManager::textRenderer->renderText("5TH " + toStringFill(highScores[4],6) + "     " + highScoresNames[4], 
            glm::vec2(3.5,11.5), 0.5f, glm::vec3(0, 1, 1));
    }
}
