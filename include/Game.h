#ifndef GAME_H
#define GAME_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Texture.h"
#include "SpriteFrame.h"

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
    GAME_EXIT
};

class Game
{
    private:
    public:
        static GLint score;
        static GLint lifes;
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

        std::map<GLint, GLint> keys;    // <Key, Action{PRESS(1),REPEAT(2),RELEASE(0)}>

        GLuint WIDTH, HEIGHT;

        GLFWwindow* window;

        GLboolean _3DEnabled, musicEnabled, soundsEnabled;
        // Constructor/Destructor
        Game(GLFWwindow* window, GLuint width, GLuint height);
        virtual ~Game();

        // Initialize game state (load all shaders/textures/levels)
        void init();

        //GameLoop
        void proccessInput();
        void update();
        void render(GLfloat interpolation);

};

#endif // GAME_H
