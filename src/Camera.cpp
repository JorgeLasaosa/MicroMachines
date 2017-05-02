#include "Camera.h"

#include <iostream>

/* PUBLIC METHODS */
Camera::Camera(const GLfloat theta, const GLfloat phi, const GLfloat distance, glm::vec3 positionToLook, const GLint windowWidth, const GLint windowHeight)
    : movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), active(false),
      theta(theta), phi(phi), distance(distance),
      windowWidth(windowWidth), windowHeight(windowHeight), positionToLook(positionToLook)
{
    this->squareSize = windowHeight / 18.0f;
    this->worldUp = glm::vec3(0,1,0);
    this->updateCameraVectors();
}

/**
 *  Returns the view matrix calculated using Eular Angles and the LookAt Matrix
 */
const glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->position * squareSize, this->position * squareSize + this->front, this->up);
//    return glm::lookAt(this->position * squareSize, this->positionToLook * squareSize + this->front, glm::vec3(0,1,0));
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
void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLfloat interpolation)
{
    xOffset *= this->mouseSensitivity * interpolation;
    yOffset *= this->mouseSensitivity * interpolation;

    this->theta += xOffset;
    this->phi += yOffset;

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
//    std::cout << std::endl << "Camera position: (" << this->position.x << ", " << this->position.y << ", " << this->position.z << ")" << std::endl;
//    std::cout << "Camera up: (" << this->up.x << ", " << this->up.y << ", " << this->up.z << ")" << std::endl;
//    std::cout << "Camera yaw: " << this->yaw << std::endl;
//    std::cout << "Camera pitch: " << this->pitch << std::endl << std::endl;
    std::cout << "Theta: " << theta << std::endl;
    std::cout << "Phi: " << phi << std::endl;
}

///**
// *  Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
// */
//void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLfloat interpolation)
//{
//    xOffset *= this->mouseSensitivity;
//    yOffset *= this->mouseSensitivity;
//
//    this->yaw   += xOffset;
//    this->pitch += yOffset;
//
//    GLfloat phi = pitch;
//    GLfloat theta = yaw;
//
//    this->position.x = positionToLook.x + distance*cos(phi)*sin(theta);
//    this->position.y = positionToLook.y + distance*sin(phi)*sin(theta);
//    this->position.z = positionToLook.z + distance*cos(theta);
//    // Update Front, Right and Up Vectors using the updated Eular angles
//    //this->updateCameraVectors();
//    std::cout << std::endl << "Camera position: (" << this->position.x << ", " << this->position.y << ", " << this->position.z << ")" << std::endl;
//    std::cout << "Camera up: (" << this->up.x << ", " << this->up.y << ", " << this->up.z << ")" << std::endl;
//    std::cout << "Camera yaw: " << this->yaw << std::endl;
//    std::cout << "Camera pitch: " << this->pitch << std::endl << std::endl;
//}

/**
 *  Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
 */
void Camera::processMouseScroll(GLfloat yOffset)
{
//    if (this->zoom >= 1.0f && this->zoom <= 45.0f)
//        this->zoom -= yOffset;
//    if (this->zoom <= 1.0f)
//        this->zoom = 1.0f;
//    if (this->zoom >= 45.0f)
//        this->zoom = 45.0f;
    if (this->distance <= 5.0f) {
        this->distance = 5.0f;
    }
    this->updateCameraVectors();
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

void Camera::copyValues(const Camera& camera) {
    this->position = camera.position;
    this->front = camera.front;
    this->up = camera.up;
    this->right = camera.right;
    this->yaw = camera.yaw;
    this->pitch = camera.pitch;
    this->updateCameraVectors();
}

/* PRIVATE METHODS */

/**
 * Calculates the front vector from the Camera's Eular angles
 */
void Camera::updateCameraVectors() {
//    std::cout << "Position: " << this->position.x << ", " << this->position.y << ", " << this->position.z << std::endl;
    this->position.x = this->positionToLook.x + sin(phi) * cos(theta) * distance;
    this->position.y = this->positionToLook.y + cos(phi) * distance;
    this->position.z = this->positionToLook.z + sin(phi) * sin(theta) * distance;

    // Calculate the new front vector
    glm::vec3 front = this->positionToLook - this->position;
//    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
//    front.y = sin(glm::radians(this->pitch));
//    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);

    // Also re-calculate right and up vectors
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
