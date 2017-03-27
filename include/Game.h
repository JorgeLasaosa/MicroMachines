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
    GAME_WIN
};

class Game
{
    private:
    public:
        static GLint score;
        // Game state
        GameState state;
        GLfloat time_step;

        Texture introSprite;
        SpriteFrame introSpriteFrame;

        std::map<GLint, GLint> keys;    // <Key, Action{PRESS(1),REPEAT(2),RELEASE(0)}>

        GLuint WIDTH, HEIGHT;

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

};

#endif // GAME_H
