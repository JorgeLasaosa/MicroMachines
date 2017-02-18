// GL and GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Our Classes
#include "Game.h"
#include "ResourceManager.h"    // Allows generate and store shaders and textures

#include <iostream>

// Global variables

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 700;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

GLfloat deltaTime = 0.0f;  // Time between 2 frames
GLfloat lastFrame = 0.0f;  // Time of the last frame


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
	game.init();
	game.state = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window)) {

        //Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();

		// Manage user input
		game.proccessInput(deltaTime);

		// Update Game state
		game.update(deltaTime);

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.render();

		// Swap buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources loaded using the resource manager
	ResourceManager::clear();

	glfwTerminate();

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
        if (action == GLFW_PRESS) {
            game.keys[key] = GL_TRUE;
        }

        else if (action == GLFW_RELEASE) {
            game.keys[key] = GL_FALSE;
        }
    }
}
