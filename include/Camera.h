#ifndef CAMERA_H
#define CAMERA_H

// GL includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.5f;
const GLfloat SENSITIVITY = 0.05f;
const GLfloat ZOOM = 45.0f;

class Camera
{
    public:
        /* Constructor with vectors */
        Camera(glm::vec3 position,const GLint windowWidth,  const GLint windowHeight, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

        /* Returns the view matrix calculated using Eular Angles and the LookAt Matrix */
        const glm::mat4 getViewMatrix() const;

        /* Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems) */
        void processKeyboard(Camera_Movement direction, GLfloat deltaTime);

        /* Processes input received from a mouse input system. Expects the offset value in both the x and y direction. */
        void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

        /* Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis */
        void processMouseScroll(GLfloat yOffset);

        /* activate the camera projection */
        void enable();

        /* deactivate the camera projection */
        void disable();

        /* check if camera projection is enabled */
        GLboolean isEnabled();

        /* get Orthogonal projection */
        glm::mat4 getOrthogonal();

        /* get Perspective projection */
        glm::mat4 getPerspective();

        /* Getter zoom */
        const GLfloat getZoom() const {return this->zoom;}

    private:
        // Camera Attributes
        glm::vec3 position;
        glm::vec3 front;    // Camera Z axis
        glm::vec3 up;       // Camera Y axis
        glm::vec3 right;    // Camera X axis

        GLint windowWidth, windowHeight;
        GLfloat squareSize;

        glm::vec3 worldUp;

        // Eular angles
        GLfloat yaw;
        GLfloat pitch;

        // Camera Options
        GLfloat movementSpeed;
        GLfloat mouseSensitivity;
        GLfloat zoom;
        GLboolean active;

        // Calculates the front vector from the Camera's Eular angles
        void updateCameraVectors();
};

#endif // CAMERA_H
