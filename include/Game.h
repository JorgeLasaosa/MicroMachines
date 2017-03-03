#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_GEN_LEVEL,
    GAME_GEN_START,
    GAME_MENU,
    GAME_WIN
};

class Game
{
    private:
    public:
        // Game state
        GameState state;
        GLboolean keys[1024];
        GLfloat time_step = 0;

        GLuint WIDTH, HEIGHT;

        // Constructor/Destructor
        Game(GLuint width, GLuint height);
        virtual ~Game();

        // Initialize game state (load all shaders/textures/levels)
        void init();

        //GameLoop
        void proccessInput(GLfloat dt);
        void update(GLfloat dt);
        void render();

};

#endif // GAME_H
