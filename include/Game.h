#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

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
        GLfloat time_step = 0;

        std::map<GLint, GLint> keys;    // <Key, Action (Press or Release)

        GLuint WIDTH, HEIGHT;

        // Constructor/Destructor
        Game(GLuint width, GLuint height);
        virtual ~Game();

        // Initialize game state (load all shaders/textures/levels)
        void init();

        //GameLoop
        void proccessInput();
        void update();
        void render(GLfloat interpolation);

};

#endif // GAME_H
