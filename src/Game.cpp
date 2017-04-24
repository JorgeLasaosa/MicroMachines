
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "GameLevel.h"
#include "Player.h"
#include "TextRenderer.h"
#include "Menu.h"
#include "Cube3DRenderer.h"

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

// Keys
GLboolean modifyingKeys = false;
GLint actionKey = GLFW_KEY_LEFT_CONTROL;
GLint pauseKey = GLFW_KEY_ESCAPE;
GLint leftKey = GLFW_KEY_LEFT;
GLint rightKey = GLFW_KEY_RIGHT;
GLint upKey = GLFW_KEY_UP;
GLint downKey = GLFW_KEY_DOWN;
GLint Game::lastKey = GLFW_KEY_DOWN;

GLint actionKeyTmp = GLFW_KEY_LEFT_CONTROL;
GLint pauseKeyTmp = GLFW_KEY_ESCAPE;
GLint leftKeyTmp = GLFW_KEY_LEFT;
GLint rightKeyTmp = GLFW_KEY_RIGHT;
GLint upKeyTmp = GLFW_KEY_UP;
GLint downKeyTmp = GLFW_KEY_DOWN;

// Menus
Menu* mainMenu;
Menu* configMenu;
Menu* controlMenu;
Menu* pauseMenu;
Menu* activeMenu;   // Pointer to active menu (Main, Config)

GLint Game::score = 0;
GLint Game::lifes = 2;
GLint Game::timeLevel = 0;
GLint timeLevelStep = 0;

GLboolean Game::musicEnabled = true;
GLboolean Game::soundsEnabled = true;
GLboolean Game::mode3D = true;

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

Component3D* pengo3D;
Component3D* pengoArmL;
Component3D* pengoArmR;
Component3D* pengoFeetL;
Component3D* pengoFeetR;

Component3D* snobee3D;
Component3D* snobeeArm1L;
Component3D* snobeeArm2L;
Component3D* snobeeHandL;
Component3D* snobeeArm1R;
Component3D* snobeeArm2R;
Component3D* snobeeHandR;

Cube3DRenderer* cube3DRenderer;

GLfloat scalePengo;
GLfloat rot = 0;

// Cheat list
GLboolean Game::cheat_Invincible = false;
GLboolean Game::cheat_InfiniteLifes = false;

// Game Constructor
Game::Game(GLFWwindow* window, GLuint width, GLuint height, Camera* camera)
    : window(window), WIDTH(width), HEIGHT(height), time_step(0), maxEggsInLevel(6), camera(camera) {}

// Game Destructor
Game::~Game() {
	delete renderer;
	delete cube3DRenderer;
	delete level;
	delete mainMenu;
	delete configMenu;
}

static std::string getKeyName(GLint key){
    std::string str;
    switch(key) {
        case GLFW_KEY_UNKNOWN: str = "UNK";
        break;
        case GLFW_KEY_SPACE: str = "SPACE";
        break;
        case GLFW_KEY_APOSTROPHE: str = "APOS";
        break;
        case GLFW_KEY_COMMA: str = ",";
        break;
        case GLFW_KEY_MINUS: str = "-";
        break;
        case GLFW_KEY_PERIOD: str = "PER";
        break;
        case GLFW_KEY_SLASH: str = "/";
        break;
        case GLFW_KEY_0: str = "0";
        break;
        case GLFW_KEY_1: str = "1";
        break;
        case GLFW_KEY_2: str = "2";
        break;
        case GLFW_KEY_3: str = "3";
        break;
        case GLFW_KEY_4: str = "4";
        break;
        case GLFW_KEY_5: str = "5";
        break;
        case GLFW_KEY_6: str = "6";
        break;
        case GLFW_KEY_7: str = "7";
        break;
        case GLFW_KEY_8: str = "8";
        break;
        case GLFW_KEY_9: str = "9";
        break;
        case GLFW_KEY_SEMICOLON: str = "'";
        break;
        case GLFW_KEY_EQUAL: str = "=";
        break;
        case GLFW_KEY_A: str = "A";
        break;
        case GLFW_KEY_B: str = "B";
        break;
        case GLFW_KEY_C: str = "C";
        break;
        case GLFW_KEY_D: str = "D";
        break;
        case GLFW_KEY_E: str = "E";
        break;
        case GLFW_KEY_F: str = "F";
        break;
        case GLFW_KEY_G: str = "G";
        break;
        case GLFW_KEY_H: str = "H";
        break;
        case GLFW_KEY_I: str = "I";
        break;
        case GLFW_KEY_J: str = "J";
        break;
        case GLFW_KEY_K: str = "K";
        break;
        case GLFW_KEY_L: str = "L";
        break;
        case GLFW_KEY_M: str = "M";
        break;
        case GLFW_KEY_N: str = "N";
        break;
        case GLFW_KEY_O: str = "O";
        break;
        case GLFW_KEY_P: str = "P";
        break;
        case GLFW_KEY_Q: str = "Q";
        break;
        case GLFW_KEY_R: str = "R";
        break;
        case GLFW_KEY_S: str = "S";
        break;
        case GLFW_KEY_T: str = "T";
        break;
        case GLFW_KEY_U: str = "U";
        break;
        case GLFW_KEY_V: str = "V";
        break;
        case GLFW_KEY_W: str = "W";
        break;
        case GLFW_KEY_X: str = "X";
        break;
        case GLFW_KEY_Y: str = "Y";
        break;
        case GLFW_KEY_Z: str = "Z";
        break;
        case GLFW_KEY_LEFT_BRACKET: str = "{";
        break;
        case GLFW_KEY_BACKSLASH: str = "\\";
        break;
        case GLFW_KEY_RIGHT_BRACKET: str = "}";
        break;
        case GLFW_KEY_GRAVE_ACCENT: str = "`";
        break;
        case GLFW_KEY_WORLD_1: str = "W1";
        break;
        case GLFW_KEY_WORLD_2: str = "W2";
        break;
        case GLFW_KEY_ESCAPE: str = "ESC";
        break;
        case GLFW_KEY_ENTER: str = "ENTER";
        break;
        case GLFW_KEY_TAB: str = "TAB";
        break;
        case GLFW_KEY_BACKSPACE: str = "BS";
        break;
        case GLFW_KEY_INSERT: str = "INS";
        break;
        case GLFW_KEY_DELETE: str = "DEL";
        break;
        case GLFW_KEY_RIGHT: str = "RIGHT";
        break;
        case GLFW_KEY_LEFT: str = "LEFT";
        break;
        case GLFW_KEY_DOWN: str = "DOWN";
        break;
        case GLFW_KEY_UP: str = "UP";
        break;
        case GLFW_KEY_PAGE_UP: str = "PAG.UP";
        break;
        case GLFW_KEY_PAGE_DOWN: str = "PAG.DOWN";
        break;
        case GLFW_KEY_HOME: str = "HOME";
        break;
        case GLFW_KEY_END: str = "END";
        break;
        case GLFW_KEY_CAPS_LOCK: str = "C.LOCK";
        break;
        case GLFW_KEY_SCROLL_LOCK: str = "S.LOCK";
        break;
        case GLFW_KEY_NUM_LOCK: str = "N.LOCK";
        break;
        case GLFW_KEY_PRINT_SCREEN: str = "IMPR";
        break;
        case GLFW_KEY_PAUSE: str = "PAUSE";
        break;
        case GLFW_KEY_F1: str = "F1";
        break;
        case GLFW_KEY_F2: str = "F2";
        break;
        case GLFW_KEY_F3: str = "F3";
        break;
        case GLFW_KEY_F4: str = "F4";
        break;
        case GLFW_KEY_F5: str = "F5";
        break;
        case GLFW_KEY_F6: str = "F6";
        break;
        case GLFW_KEY_F7: str = "F7";
        break;
        case GLFW_KEY_F8: str = "F8";
        break;
        case GLFW_KEY_F9: str = "F9";
        break;
        case GLFW_KEY_F10: str = "F10";
        break;
        case GLFW_KEY_F11: str = "F11";
        break;
        case GLFW_KEY_F12: str = "F12";
        break;
        case GLFW_KEY_F13: str = "F13";
        break;
        case GLFW_KEY_F14: str = "F14";
        break;
        case GLFW_KEY_F15: str = "F15";
        break;
        case GLFW_KEY_F16: str = "F16";
        break;
        case GLFW_KEY_F17: str = "F17";
        break;
        case GLFW_KEY_F18: str = "F18";
        break;
        case GLFW_KEY_F19: str = "F19";
        break;
        case GLFW_KEY_F20: str = "F20";
        break;
        case GLFW_KEY_F21: str = "F21";
        break;
        case GLFW_KEY_F22: str = "F22";
        break;
        case GLFW_KEY_F23: str = "F23";
        break;
        case GLFW_KEY_F24: str = "F24";
        break;
        case GLFW_KEY_F25: str = "F25";
        break;
        case GLFW_KEY_KP_0: str = "KP0";
        break;
        case GLFW_KEY_KP_1: str = "KP1";
        break;
        case GLFW_KEY_KP_2: str = "KP2";
        break;
        case GLFW_KEY_KP_3: str = "KP3";
        break;
        case GLFW_KEY_KP_4: str = "KP4";
        break;
        case GLFW_KEY_KP_5: str = "KP5";
        break;
        case GLFW_KEY_KP_6: str = "KP6";
        break;
        case GLFW_KEY_KP_7: str = "KP7";
        break;
        case GLFW_KEY_KP_8: str = "KP8";
        break;
        case GLFW_KEY_KP_9: str = "KP9";
        break;
        case GLFW_KEY_KP_DECIMAL: str = "KPDEC";
        break;
        case GLFW_KEY_KP_DIVIDE: str = "KPDIV";
        break;
        case GLFW_KEY_KP_MULTIPLY: str = "KPMUL";
        break;
        case GLFW_KEY_KP_SUBTRACT: str = "KPSUB";
        break;
        case GLFW_KEY_KP_ADD: str = "KPADD";
        break;
        case GLFW_KEY_KP_ENTER: str = "KPENTER";
        break;
        case GLFW_KEY_KP_EQUAL: str = "KPEQ";
        break;
        case GLFW_KEY_LEFT_SHIFT: str = "SHIFT";
        break;
        case GLFW_KEY_LEFT_CONTROL: str = "CTRL";
        break;
        case GLFW_KEY_LEFT_ALT: str = "ALT";
        break;
        case GLFW_KEY_LEFT_SUPER: str = "SUPER";
        break;
        case GLFW_KEY_RIGHT_SHIFT: str = "RSHIFT";
        break;
        case GLFW_KEY_RIGHT_CONTROL: str = "RCTRL";
        break;
        case GLFW_KEY_RIGHT_ALT: str = "RALT";
        break;
        case GLFW_KEY_RIGHT_SUPER: str = "RSUPER";
        break;
        case GLFW_KEY_MENU: str = "MENU";
        break;
    }
    return str;
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
    ResourceManager::loadShaderFromFile("shaders/model3D.vs", "shaders/model3D.frag", nullptr, "model3D");
	ResourceManager::loadShaderFromFile("shaders/text.vs", "shaders/text.frag", nullptr, "text");
	ResourceManager::loadShaderFromFile("shaders/cube3D.vs", "shaders/sprite.frag", nullptr, "cube3D");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->WIDTH), static_cast<GLfloat>(this->HEIGHT), 0.0f, -1.0f, 1.0f);

	// Sprite shader
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);

	// Text shader
    ResourceManager::getShader("text").use().setMatrix4("projection", projection);

    // Model shader
    ResourceManager::getShader("model3D").use().setInteger("image", 0);
    ResourceManager::getShader("model3D").setMatrix4("projection", projection);

	// Load textures
	ResourceManager::loadTexture("img/introPengo.png", GL_TRUE, "intro");
	ResourceManager::loadTexture("img/PengoMenuAnimation.png", GL_TRUE, "menuAnim");
	ResourceManager::loadTexture("img/walls.png", GL_TRUE, "walls");
	ResourceManager::loadTexture("img/diamond/diamond-shiny.png", GL_TRUE, "diamond-shiny");
	ResourceManager::loadTexture("img/blocks.png", GL_TRUE, "blocks");
	ResourceManager::loadTexture("img/creatures.png", GL_TRUE, "creatures");
	ResourceManager::loadTexture("img/indicators&eggs.png", GL_TRUE, "indicators-n-eggs");
	ResourceManager::loadTexture("img/pauseMenuBackground.png", GL_TRUE, "pause-background");

	// SpriteRenderer
	Shader spriteShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(spriteShader, this->WIDTH, this->HEIGHT);

	// Cube3DRenderer
	Shader cube3DShader = ResourceManager::getShader("cube3D");
    cube3DRenderer = new Cube3DRenderer(cube3DShader, this->WIDTH, this->HEIGHT, this->camera);

	// Component3D
    Shader modelShader = ResourceManager::getShader("model3D");

    ResourceManager::loadMesh("models/Pengo.mply", modelShader, this->camera, this->WIDTH, this->HEIGHT, "pengo");
    ResourceManager::loadMesh("models/PengoArmLeft.mply", modelShader, this->camera, this->WIDTH, this->HEIGHT, "pengoArm");
    ResourceManager::loadMesh("models/PengoFeetLeft.mply", modelShader, this->camera, this->WIDTH, this->HEIGHT, "pengoFeet");
    ResourceManager::loadMesh("models/Snobee.mply", modelShader, this->camera, this->WIDTH, this->HEIGHT, "snobee");
    ResourceManager::loadMesh("models/SnobeeArm.mply", modelShader, this->camera, this->WIDTH, this->HEIGHT, "snobeeArm");
    ResourceManager::loadMesh("models/SnobeeHandL.mply", modelShader, this->camera, this->WIDTH, this->HEIGHT, "snobeeHand");
    scalePengo = this->HEIGHT / 18.0f;
    pengo3D = new Component3D(ResourceManager::getMesh("pengo"));
    pengo3D->setPosition(glm::vec3(7,12,0) * scalePengo);
    pengo3D->setScale(glm::vec3(1,-1,0.001f) * scalePengo);
    //pengo3D->setScale(glm::vec3(1,-1,1) * scalePengo);

    pengoArmL = new Component3D(ResourceManager::getMesh("pengoArm"));
    pengoArmL->setPosition(glm::vec3(1.18751,3.59175,0)); // Relative to Pengo
    pengoArmL->setParent(pengo3D);

    pengoArmR = new Component3D(ResourceManager::getMesh("pengoArm"));
    pengoArmR->setPosition(glm::vec3(-1.18751,3.59175,0)); // Relative to Pengo
    pengoArmR->setScale(glm::vec3(-1,1,1));
    pengoArmR->setParent(pengo3D);

    pengoFeetL = new Component3D(ResourceManager::getMesh("pengoFeet"));
    pengoFeetL->setPosition(glm::vec3(0.62,1.45,0.34)); // Relative to Pengo
    pengoFeetL->setParent(pengo3D);

    pengoFeetR = new Component3D(ResourceManager::getMesh("pengoFeet"));
    pengoFeetR->setPosition(glm::vec3(-0.62,1.45,0.34)); // Relative to Pengo
    pengoFeetR->setScale(glm::vec3(-1,1,1));
    pengoFeetR->setParent(pengo3D);



    snobee3D = new Component3D(ResourceManager::getMesh("snobee"));
    snobee3D->setPosition(glm::vec3(7,12,0) * scalePengo);
    snobee3D->setScale(glm::vec3(1,-1,0.001f) * scalePengo);

    snobeeArm1L = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm1L->setPosition(glm::vec3(-0.816,0,-0.044)); // Relative to Snobee
    snobeeArm1L->setParent(snobee3D);

    snobeeArm1R = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm1R->setPosition(glm::vec3(0.816,0,-0.044)); // Relative to Snobee
    snobeeArm1R->setScale(glm::vec3(-1,1,1));
    snobeeArm1R->setParent(snobee3D);

    snobeeArm2L = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm2L->setPosition(glm::vec3(-0.7356,0,0)); // Relative to SnobeeArm1L
    snobeeArm2L->setRotation(glm::vec3(0,90,0)); // Relative to SnobeeArm1L
    snobeeArm2L->setParent(snobeeArm1L);

    snobeeArm2R = new Component3D(ResourceManager::getMesh("snobeeArm"));
    snobeeArm2R->setPosition(glm::vec3(-0.7356,0,0)); // Relative to SnobeeArm1R
    snobeeArm2R->setRotation(glm::vec3(0,90,0)); // Relative to SnobeeArm1R
    snobeeArm2R->setParent(snobeeArm1R);


    snobeeHandL = new Component3D(ResourceManager::getMesh("snobeeHand"));
    snobeeHandL->setPosition(glm::vec3(-1.12,0,0)); // Relative to SnobeeArmL
    snobeeHandL->setParent(snobeeArm2L);

    snobeeHandR = new Component3D(ResourceManager::getMesh("snobeeHand"));
    snobeeHandR->setPosition(glm::vec3(-1.12,0,0)); // Relative to SnobeeArmL
    snobeeHandR->setParent(snobeeArm2R);

	Shader textShader = ResourceManager::getShader("text");
	ResourceManager::initTextRenderer(textShader, this->WIDTH, this->HEIGHT);


	// Store filenames of all levels and load a random level
	allLevels.push_back("levels/level1.txt");
    allLevels.push_back("levels/level2.txt");
    allLevels.push_back("levels/level3.txt");
    // allLevels.push_back("levels/level_testBonus.txt");
    // allLevels.push_back("levels/level_testPushNear.txt");
    //allLevels.push_back("levels/level_testPushNear2.txt");

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
	configMenuOptions.push_back({"GRAPHICS  3D", glm::vec3(0.0f, 1.0f, 1.0f)});
	configMenuOptions.push_back({"MUSIC     ON", glm::vec3(0.0f, 1.0f, 1.0f)});
    configMenuOptions.push_back({"SOUNDS    ON", glm::vec3(0.0f, 1.0f, 1.0f)});
    configMenuOptions.push_back({"CONTROLS", glm::vec3(0.0f, 1.0f, 1.0f)});
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
	pauseMenuOptions.push_back({"GRAPHICS  3D", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"MUSIC     ON", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"SOUNDS    ON", glm::vec3(0.0f, 1.0f, 1.0f)});
	pauseMenuOptions.push_back({"EXIT GAME", glm::vec3(0.0f, 1.0f, 1.0f)});

	pauseMenu->setOptions(pauseMenuOptions);

    controlMenu = new Menu(glm::vec2(3.0f, 5.5f));

    std::vector<Menu::MenuOption> controlMenuOptions;
    std::string actionS = "ACTION  ";
    std::string upS     = "UP      ";
    std::string leftS   = "LEFT    ";
    std::string downS   = "DOWN    ";
    std::string rightS  = "RIGHT   ";
    std::string pauseS  = "PAUSE   ";
    controlMenuOptions.push_back({actionS + getKeyName(actionKey), glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({upS + getKeyName(upKey), glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({leftS + getKeyName(leftKey), glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({downS + getKeyName(downKey), glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({rightS + getKeyName(rightKey), glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({pauseS + getKeyName(pauseKey), glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({"RESET", glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({"SAVE", glm::vec3(0.0f, 1.0f, 1.0f)});
    controlMenuOptions.push_back({"DISCARD", glm::vec3(0.0f, 1.0f, 1.0f)});

    controlMenu->setOptions(controlMenuOptions);

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
        else if(level->state == LEVEL_LOSE2){
            player->update();
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
            keyPressedInRecords = false;
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
        } else if(cheat.compare("Earthquake")==0) {
            cout << "Or.... 'Icequake'?" << endl;
            for(auto &i : level->field) {
                for (auto &j : i) {
                    if (j!=nullptr){

                        Iceblock* block = dynamic_cast< Iceblock* >(j);
                        if (block!=nullptr) {
                            block->disintegrate(level,false);
                        }
                    }
                }
            }
        } else {
            cout << "Unknow cheat" << endl;
        }
    } else if (this->keys[GLFW_KEY_C] == GLFW_RELEASE) {
        keyCheatPressed = false;
    }

	if (this->state == GAME_INTRO && this->keys[actionKey] == GLFW_PRESS ) {
        this->state = GAME_MENU;
        keyPressedInMenu = true;
	}

	// IN MAIN MENU
	if (this->state == GAME_MENU) {
        if (!modifyingKeys && !keyPressedInMenu){
            if (this->keys[downKey] == GLFW_PRESS) {
                activeMenu->nextOption();
                keyPressedInMenu = true;
            }
            else if(this->keys[upKey] == GLFW_PRESS) {
                activeMenu->previousOption();
                keyPressedInMenu = true;
            }
            else if (this->keys[actionKey] == GLFW_PRESS) {
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
                            if (mode3D) {
                                configMenu->options[0].text = "GRAPHICS  2D";
                                pauseMenu->options[1].text = "GRAPHICS  2D";
                                mode3D = false;
                            }
                            else {
                                configMenu->options[0].text = "GRAPHICS  3D";
                                pauseMenu->options[1].text = "GRAPHICS  3D";
                                mode3D = true;
                            }
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
                        case 3: // GO TO CONTROLS MENU
                            activeMenu = controlMenu;
                        break;
                        case 4: // GO BACK TO MAIN MENU
                            activeMenu = mainMenu;
                        break;
                    }
                }
                else if (activeMenu == controlMenu) {
                    switch(controlMenu->getSelector()) {
                        case 0: // ACTION
                        case 1: // UP
                        case 2: // LEFT
                        case 3: // DOWN
                        case 4: // RIGHT
                        case 5: // PAUSE
                            modifyingKeys = true;
                            controlMenu->options[controlMenu->getSelector()].color = glm::vec3(1.0f, 0.0f, 0.0f);
                        break;
                        case 6: {// RESET
                            actionKeyTmp = GLFW_KEY_LEFT_CONTROL;
                            pauseKeyTmp = GLFW_KEY_ESCAPE;
                            leftKeyTmp = GLFW_KEY_LEFT;
                            rightKeyTmp = GLFW_KEY_RIGHT;
                            upKeyTmp = GLFW_KEY_UP;
                            downKeyTmp = GLFW_KEY_DOWN;
                            std::string actionS = "ACTION  ";
                            controlMenu->options[0].text = actionS + getKeyName(actionKeyTmp);
                            std::string upS     = "UP      ";
                            controlMenu->options[1].text = upS + getKeyName(upKeyTmp);
                            std::string leftS   = "LEFT    ";
                            controlMenu->options[2].text = leftS + getKeyName(leftKeyTmp);
                            std::string downS   = "DOWN    ";
                            controlMenu->options[3].text = downS + getKeyName(downKeyTmp);
                            std::string rightS  = "RIGHT   ";
                            controlMenu->options[4].text = rightS + getKeyName(rightKeyTmp);
                            std::string pauseS  = "PAUSE   ";
                            controlMenu->options[5].text = pauseS + getKeyName(pauseKeyTmp);
                        }
                        break;
                        case 7: {// SAVE
                            actionKey = actionKeyTmp;
                            pauseKey = pauseKeyTmp;
                            leftKey = leftKeyTmp;
                            rightKey = rightKeyTmp;
                            upKey = upKeyTmp;
                            downKey = downKeyTmp;
                            activeMenu = configMenu;
                        }
                        break;
                        case 8: // BACK
                            activeMenu = configMenu;
                        break;
                    }
                }
            }
        }
        else if (this->keys[lastKey] == GLFW_PRESS && !keyPressedInMenu){
            GLint tmpKey;
            GLint selection = controlMenu->getSelector();
            switch(selection) {
                case 0: {// ACTION
                    tmpKey = actionKeyTmp;
                    actionKeyTmp = lastKey;
                    modifyingKeys = false;
                    std::string actionS = "ACTION  ";
                    controlMenu->options[0].text = actionS + getKeyName(actionKeyTmp);
                    controlMenu->options[0].color = glm::vec3(1.0f, 0.8f, 0.0f);
                }
                break;
                case 1: {// UP
                    tmpKey = upKeyTmp;
                    upKeyTmp = lastKey;
                    modifyingKeys = false;
                    std::string upS     = "UP      ";
                    controlMenu->options[1].text = upS + getKeyName(upKeyTmp);
                    controlMenu->options[1].color = glm::vec3(1.0f, 0.8f, 0.0f);
                }
                break;
                case 2: {// LEFT
                    tmpKey = leftKeyTmp;
                    leftKeyTmp = lastKey;
                    modifyingKeys = false;
                    std::string leftS   = "LEFT    ";
                    controlMenu->options[2].text = leftS + getKeyName(leftKeyTmp);
                    controlMenu->options[2].color = glm::vec3(1.0f, 0.8f, 0.0f);
                }
                break;
                case 3: {// DOWN
                    tmpKey = downKeyTmp;
                    downKeyTmp = lastKey;
                    modifyingKeys = false;
                    std::string downS   = "DOWN    ";
                    controlMenu->options[3].text = downS + getKeyName(downKeyTmp);
                    controlMenu->options[3].color = glm::vec3(1.0f, 0.8f, 0.0f);
                }
                break;
                case 4: {// RIGHT
                    tmpKey = rightKeyTmp;
                    rightKeyTmp = lastKey;
                    modifyingKeys = false;
                    std::string rightS  = "RIGHT   ";
                    controlMenu->options[4].text = rightS + getKeyName(rightKeyTmp);
                    controlMenu->options[4].color = glm::vec3(1.0f, 0.8f, 0.0f);
                }
                break;
                case 5: {// PAUSE
                    tmpKey = pauseKeyTmp;
                    pauseKeyTmp = lastKey;
                    modifyingKeys = false;
                    std::string pauseS  = "PAUSE   ";
                    controlMenu->options[5].text = pauseS + getKeyName(pauseKeyTmp);
                    controlMenu->options[5].color = glm::vec3(1.0f, 0.8f, 0.0f);
                }
                break;
            }
            if (actionKeyTmp == lastKey && selection!=0){
                actionKeyTmp = tmpKey;
                std::string actionS = "ACTION  ";
                controlMenu->options[0].text = actionS + getKeyName(actionKeyTmp);
            }
            else if (upKeyTmp == lastKey && selection!=1){
                upKeyTmp = tmpKey;
                std::string upS     = "UP      ";
                controlMenu->options[1].text = upS + getKeyName(upKeyTmp);
            }
            else if (leftKeyTmp == lastKey && selection!=2){
                leftKeyTmp = tmpKey;
                std::string leftS   = "LEFT    ";
                controlMenu->options[2].text = leftS + getKeyName(leftKeyTmp);
            }
            else if (downKeyTmp == lastKey && selection!=3){
                downKeyTmp = tmpKey;
                std::string downS   = "DOWN    ";
                controlMenu->options[3].text = downS + getKeyName(downKeyTmp);
            }
            else if (rightKeyTmp == lastKey && selection!=4){
                rightKeyTmp = tmpKey;
                std::string rightS  = "RIGHT   ";
                controlMenu->options[4].text = rightS + getKeyName(rightKeyTmp);
            }
            else if (pauseKeyTmp == lastKey && selection!=5){
                pauseKeyTmp = tmpKey;
                std::string pauseS  = "PAUSE   ";
                controlMenu->options[5].text = pauseS + getKeyName(pauseKeyTmp);
            }
            keyPressedInMenu = true;
        }
        else if (this->keys[downKey] == GLFW_RELEASE && this->keys[upKey] == GLFW_RELEASE
                 && this->keys[actionKey] == GLFW_RELEASE){
            keyPressedInMenu = false;
        }
    }

    // IN PAUSE MENU
    else if (this->state == GAME_PAUSE_MENU) {
        if (this->keys[downKey] == GLFW_PRESS && !keyPressedInMenu) {
            pauseMenu->nextOption();
            keyPressedInMenu = true;
        }
        else if(this->keys[upKey] == GLFW_PRESS && !keyPressedInMenu) {
            pauseMenu->previousOption();
            keyPressedInMenu = true;
        }
        else if (this->keys[actionKey] == GLFW_PRESS && !keyPressedInMenu) {
            keyPressedInMenu = true;
            switch(pauseMenu->getSelector()) {
                case 0: // CONTINUE GAME
                    this->state = GAME_ACTIVE;
                    ResourceManager::musicEngine->setAllSoundsPaused(false);
                    ResourceManager::soundEngine->setAllSoundsPaused(false);
                    keyActionPressed = true;
                break;
                case 1: // GRAPHICS 2D/3D
                    if (mode3D) {
                        configMenu->options[0].text = "GRAPHICS  2D";
                        pauseMenu->options[1].text = "GRAPHICS  2D";
                        mode3D = false;
                    }
                    else {
                        configMenu->options[0].text = "GRAPHICS  3D";
                        pauseMenu->options[1].text = "GRAPHICS  3D";
                        mode3D = true;
                    }
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
        else if (this->keys[downKey] == GLFW_RELEASE && this->keys[upKey] == GLFW_RELEASE
                 && this->keys[actionKey] == GLFW_RELEASE){
            keyPressedInMenu = false;
        }
    }

	// IN GAME
	else if (this->state == GAME_ACTIVE) {
        if(this->keys[pauseKey] == GLFW_PRESS && !keyPausePressed) {
            keyPausePressed = true;
            ResourceManager::musicEngine->setAllSoundsPaused(true);
            ResourceManager::soundEngine->setAllSoundsPaused(true);
            this->state = GAME_PAUSE_MENU;
        }
        else if (this->keys[pauseKey] == GLFW_RELEASE) {
            keyPausePressed = false;
        }
		if(this->keys[actionKey] == GLFW_PRESS && player->state==STOPPED && !keyActionPressed) {
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
		if(this->keys[actionKey] == GLFW_PRESS && player->state!=MOVING) {
			keyActionPressed = true;
		}
		if (this->keys[actionKey] == GLFW_RELEASE) {
			keyActionPressed = false;
		}
		// Move playerboard
		glm::vec2 newPos;
		if (this->keys[upKey] >= GLFW_PRESS) {
			if (player->state==STOPPED) {//!player->isMoving
	        	player->movement = MOVE_UP;
	            newPos = player->position + glm::vec2(0, -1);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}

		if (this->keys[downKey] >= GLFW_PRESS) {
			if (player->state==STOPPED) {
	        	player->movement = MOVE_DOWN;
	            newPos = player->position + glm::vec2(0, 1);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}

		if (this->keys[leftKey] >= GLFW_PRESS) {
			if (player->state==STOPPED) {
	        	player->movement = MOVE_LEFT;
	            newPos = player->position + glm::vec2(-1, 0);
	            if(!level->checkCollision(newPos)){
	            	player->state = MOVING;
	            	player->destination = newPos;
	            }
			}
		}

		if (this->keys[rightKey] >= GLFW_PRESS) {
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
        if(this->keys[actionKey] == GLFW_PRESS  && !keyPressedInRecords) {
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
        if(this->keys[rightKey] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            rankingSetChar = (rankingSetChar + 1) % 3;
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }
        else if(this->keys[leftKey] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            rankingSetChar = (rankingSetChar - 1 + 3) % 3;
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }
        else if(this->keys[upKey] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            playerName[rankingSetChar] = (playerName[rankingSetChar] + 1) % ('Z' - 'A' + 1);
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }
        else if(this->keys[downKey] == GLFW_PRESS && !keyPressedInRecords) {
            keyPressedInRecords = true;
            playerName[rankingSetChar] = (playerName[rankingSetChar] - 1 + ('Z' - 'A' + 1)) % ('Z' - 'A' + 1);
            highScoresNames[rankingNewPos] = "";
            for(int i = 0; i <= rankingSetChar; i++){
                highScoresNames[rankingNewPos] += ((char) playerName[i] + 'A');
            }
        }

        if (this->keys[downKey] == GLFW_RELEASE && this->keys[upKey] == GLFW_RELEASE
                && this->keys[GLFW_KEY_RIGHT] == GLFW_RELEASE && this->keys[GLFW_KEY_LEFT] == GLFW_RELEASE
                 && this->keys[GLFW_KEY_LEFT_CONTROL] == GLFW_RELEASE){
            keyPressedInRecords = false;
        }
    }

    // Camera Movement
    if (keys[GLFW_KEY_W]) {
        camera->processKeyboard(FORWARD, 1);
    }

    if (keys[GLFW_KEY_A]) {
        camera->processKeyboard(LEFT, 1);
    }

    if (keys[GLFW_KEY_S]) {
        camera->processKeyboard(BACKWARD, 1);
    }

    if (keys[GLFW_KEY_D]) {
        camera->processKeyboard(RIGHT, 1);
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
        if(!mode3D) {   // 2D
            if (level->state == LEVEL_TMP) {
                level->clearFromTop(*renderer, rowsToClearFromTop);
                rowsToClearFromTop += interpolation;
            }
            else {
                level->draw(*renderer);
            }
        }
        else {  // 3D
            // El primer parametro será el renderer para pintar a pengo y los sno-bees
            level->draw(nullptr,*cube3DRenderer);
        }
    }
    else if (this->state == GAME_START_LEVEL) {
		if (!mode3D) { 	// 2D
            level->draw(*renderer);
		}
		else {		// 3D
            level->draw(nullptr, *cube3DRenderer);
		}
	}
	else if (this->state == GAME_GEN_LEVEL) {
		if(!mode3D) {		// 2D
            level->drawGenerating(*renderer);
		}
		else {		//3D
            level->drawGenerating(*cube3DRenderer);
		}
	}
	else if (this->state == GAME_MENU) {
		renderer->drawSprite(this->menuAnimSprite, glm::vec2(0,0.5f), glm::vec2(14,5), (this->menuAnimSpriteFrame));//WIDTH, HEIGHT 5.3125f
        activeMenu->drawMenu();
        rot += interpolation;
        GLfloat rotSin = glm::sin(rot/5);
        GLfloat rotSin2 = glm::sin(rot/3);

        // Move anim
        pengo3D->setRotation(glm::vec3(0.0f,rot,0.0f));
        pengo3D->setPosition(glm::vec3(7,12+glm::abs(rotSin)*0.2,0) * scalePengo);

        pengoArmL->setRotation(glm::vec3(rotSin*70,0.0f,0.0f));
        pengoArmR->setRotation(glm::vec3(-rotSin*70,0.0f,0.0f));
        pengoFeetL->setRotation(glm::vec3(-rotSin*30,0.0f,0.0f));
        pengoFeetR->setRotation(glm::vec3(rotSin*30,0.0f,0.0f));

        // Destroy Anim
        // pengo3D->setRotation(glm::vec3(15.0f,rot,0.0f));
        // pengo3D->setPosition(glm::vec3(7,12,0) * scalePengo);
        // pengoArmL->setRotation(glm::vec3(glm::sin(rot/2)*30-90,0.0f,0.0f));
        // pengoArmR->setRotation(glm::vec3(glm::sin(rot/2)*30-90,0.0f,0.0f));
        // pengoFeetL->setRotation(glm::vec3(30,0.0f,0.0f));
        // pengoFeetR->setRotation(glm::vec3(30,0.0f,0.0f));

        // Push anim
        // pengo3D->setRotation(glm::vec3(15.0f,rot,0.0f));
        // pengo3D->setPosition(glm::vec3(7,12,0) * scalePengo);
        // pengoArmL->setRotation(glm::vec3(glm::sin(rot/2)*30-90,0.0f,0.0f));
        // pengoArmR->setRotation(glm::vec3(glm::sin(rot/2)*30-90,0.0f,0.0f));
        // pengoFeetL->setRotation(glm::vec3(30,0.0f,0.0f));
        // pengoFeetR->setRotation(glm::vec3(30,0.0f,0.0f));

        // Dead anim
        // pengo3D->setRotation(glm::vec3(-90.0f,rot,0.0f));
        // pengo3D->setPosition(glm::vec3(7,12,0) * scalePengo);
        // pengoArmL->setRotation(glm::vec3(glm::sin(rot/2)*30-90,0.0f,0.0f));
        // pengoArmR->setRotation(glm::vec3(glm::sin(rot/2)*30-90,0.0f,0.0f));
        // pengoFeetL->setRotation(glm::vec3(-rotSin*30,0.0f,0.0f));
        // pengoFeetR->setRotation(glm::vec3(rotSin*30,0.0f,0.0f));

        if (rot < 360 && activeMenu!=controlMenu)
            pengo3D->draw();

        snobee3D->setRotation(glm::vec3(0.0f,rot+180,0.0f));

        if (rot>=360 && rot <360*2 && activeMenu!=controlMenu){
            snobee3D->setPosition(glm::vec3(7,12-glm::sin(rot/3-90)*0.2-2,0) * scalePengo);
            snobee3D->setScale(glm::vec3(1,-1 * (rotSin2*0.1 + 0.9), 0.001f) * scalePengo);
            snobee3D->draw(false);
        }

        if (rot>=360*2 && activeMenu!=controlMenu){
            snobeeArm1L->setRotation(glm::vec3(0.0f,-rotSin2*70,0.0f));
            snobeeArm1R->setRotation(glm::vec3(0.0f,rotSin2*70,0.0f));
            snobeeArm2R->setRotation(glm::vec3(0,-90 -rotSin2*70,0));
            snobeeArm2L->setRotation(glm::vec3(0,-90 +rotSin2*70,0));
            snobee3D->draw();
        }
        if (rot > 360*3) rot = 0;
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
