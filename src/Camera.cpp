#include "Camera.h"

#include <iostream>

/* PUBLIC METHODS */
/**
 *  Constructor with vectors
 */
 Camera::Camera(glm::vec3 position, const GLint windowWidth, const GLint windowHeight, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), active(false),
    windowWidth(windowWidth), windowHeight(windowHeight)
{
    this->squareSize = windowHeight / 18.0f;
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

/**
 *  Returns the view matrix calculated using Eular Angles and the LookAt Matrix
 */
const glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->position * squareSize, this->position * squareSize + this->front, this->up);
}

/**
 * Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 */
void Camera::processKeyboard(Camera_Movement direction, GLfloat interpolation)
{
    GLfloat velocity = this->movementSpeed * interpolation;
    if (direction == FORWARD) {
        this->position += this->front * velocity;
    }
    else if (direction == BACKWARD) {
        this->position -= this->front * velocity;
    }
    else if (direction == LEFT) {
        this->position -= this->right * velocity;
    }
    else if (direction == RIGHT) {
        this->position += this->right * velocity;
    }
//    std::cout << std::endl << "Camera position: (" << this->position.x << ", " << this->position.y << ", " << this->position.z << ")" << std::endl;
//    std::cout << "Camera up: (" << this->up.x << ", " << this->up.y << ", " << this->up.z << ")" << std::endl;
//    std::cout << "Camera yaw: " << this->yaw << std::endl;
//    std::cout << "Camera pitch: " << this->pitch << std::endl << std::endl;
}

/**
 *  Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
 */
void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw   += xOffset;
    this->pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
//    std::cout << std::endl << "Camera position: (" << this->position.x << ", " << this->position.y << ", " << this->position.z << ")" << std::endl;
//    std::cout << "Camera up: (" << this->up.x << ", " << this->up.y << ", " << this->up.z << ")" << std::endl;
//    std::cout << "Camera yaw: " << this->yaw << std::endl;
//    std::cout << "Camera pitch: " << this->pitch << std::endl << std::endl;
}

/**
 *  Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
 */
void Camera::processMouseScroll(GLfloat yOffset)
{
    if (this->zoom >= 1.0f && this->zoom <= 45.0f)
        this->zoom -= yOffset;
    if (this->zoom <= 1.0f)
        this->zoom = 1.0f;
    if (this->zoom >= 45.0f)
        this->zoom = 45.0f;
}


/**
 * Activate the camera projection
 */
void Camera::enable() {
    active = true;
}

/**
 * Deactivate the camera projection
 */
void Camera::disable() {
    active = false;
}

/**
 * Check if camera projection is enabled
 */
GLboolean Camera::isEnabled() {
    return active;
}


/**
 * Get Orthogonal projection
 */
glm::mat4 Camera::getOrthogonal(){
    return glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), static_cast<GLfloat>(windowHeight), 0.0f, -1.0f, 1.0f);
}

/**
 * Get Perspective projection
 */
glm::mat4 Camera::getPerspective() {
    return glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(windowWidth) / static_cast<GLfloat>(windowHeight), 0.1f, 100.0f * windowHeight / 18.0f);
}

/* PRIVATE METHODS */

/**
 * Calculates the front vector from the Camera's Eular angles
 */
void Camera::updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);

    // Also re-calculate right and up vectors
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
