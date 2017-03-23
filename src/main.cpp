// GL and GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Our Classes
#include "Game.h"
#include "ResourceManager.h"    // Allows generate and store shaders and textures

#include <iostream>

// Global variables
Game* game;

// Callback Functions Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

// MAIN
int main() {
	if (glfwInit() == false) {
		std::cout << "GLFW failed to initialise." << std::endl;
		return -1;
	}

	/*
	 * GLFW Configuration
	 */
	glfwWindowHint(GLFW_SAMPLES, 4);				// 4 AA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Screen dimensions
    int resolutionHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    const int SCREEN_HEIGHT = 0.95 * resolutionHeight;
    const int SCREEN_WIDTH = 14*SCREEN_HEIGHT/18;

    game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);
	// Create Window
	GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pengo", NULL, NULL);

	if (window == NULL) {
		std::cout << "Window failed to create." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

    /*
	 * Initialize GLEW
	 */
	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	// Set the required callback functions
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize game
	game->init();
	game->state = GAME_INTRO;

	// Constant Game Speed independent of variable FPS
	const GLint TICKS_PER_SECOND = 25;
	const GLfloat SKIP_TICKS = 1.0 / TICKS_PER_SECOND;
	const GLint MAX_FRAMESKIP = 5;

	GLfloat nextGameTick = glfwGetTime();
	GLint loops;
	GLfloat interpolation;

	while (!glfwWindowShouldClose(window)) {

        // Check and call events
        glfwPollEvents();

        loops = 0;
        while(glfwGetTime() > nextGameTick && loops < MAX_FRAMESKIP) {
            /**
             *  This part is executed exactly 25 times per second
             */

            // Manage user input
            game->proccessInput();

            // Update Game state
            game->update();

            nextGameTick += SKIP_TICKS;
            loops++;
        }

        interpolation = (glfwGetTime() + SKIP_TICKS - nextGameTick) / SKIP_TICKS;

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game->render(interpolation);

		// Swap buffers
		glFlush();
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources loaded using the resource manager
	ResourceManager::clear();

	glfwTerminate();
    delete game;
	return 0;
}

/*
* It is called when a key is pressed/released
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
    if (key >= 0 && key < 1024) {
        game->keys[key] = action;
    }
}
