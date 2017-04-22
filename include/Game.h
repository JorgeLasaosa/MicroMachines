#ifndef GAME_H
#define GAME_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include "Texture.h"
#include "SpriteFrame.h"
#include "Component3D.h"

// Represents the current state of the game
enum GameState {
    GAME_INTRO,
    GAME_ACTIVE,
    GAME_GEN_LEVEL,
    GAME_START_LEVEL,
    GAME_MENU,
    GAME_BONUS,
    GAME_PAUSE_MENU,
    GAME_RESPAWN,       // Respawning Pengo and enemies after Pengo's death
    GAME_WIN,
    GAME_BONUSTIME,
    GAME_RECORDS,
    GAME_OVER,
    GAME_EXIT
};

class Game
{
    private:
    public:
        static GLboolean mode3D;
        static GLint score;
        static GLint lifes;
        static GLint timeLevel;
        static GLboolean musicEnabled;
        static GLboolean soundsEnabled;
        static GLboolean _3DEnabled;
        
        // Cheat list
        static GLboolean cheat_Invincible;
        static GLboolean cheat_InfiniteLifes;

        // Game state
        GameState state;
        GLint time_step;

        Texture introSprite;
        SpriteFrame introSpriteFrame;
        Texture menuAnimSprite;
        SpriteFrame menuAnimSpriteFrame;
        Texture lifesSprite;
        SpriteFrame lifesSpriteFrame;
        SpriteFrame eggsSpriteFrame;
        GLint maxEggsInLevel;
        std::vector<GLint> highScores;
        std::vector<std::string> highScoresNames;
        std::vector<std::string> allLevels;     // Filenames of all levels
        std::vector<std::string> levelsToPlay;  // Filenames of levels pending to play

        std::map<GLint, GLint> keys;    // <Key, Action{PRESS(1),REPEAT(2),RELEASE(0)}>

        GLuint WIDTH, HEIGHT;
        char playerName [4];

        GLFWwindow* window;
        // Constructor/Destructor
        Game(GLFWwindow* window, GLuint width, GLuint height);
        virtual ~Game();

        // Initialize game state (load all shaders/textures/levels)
        void init();

        //GameLoop
        void proccessInput();
        void update();
        void render(GLfloat interpolation);
        void readHighScores();
        void writeHighScores();

};

#endif // GAME_H
