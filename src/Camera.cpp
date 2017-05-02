#include "Camera.h"
#include "Game.h"

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
}

/**
 *  Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
 */
void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLfloat interpolation)
{
    xOffset *= this->mouseSensitivity * interpolation;
    yOffset *= this->mouseSensitivity * interpolation;

    if (Game::rotatingCamera){
        this->theta += xOffset;
        this->phi += yOffset;
        if (phi > M_PI/2.0f) phi = M_PI/2.0f;
        if (phi < 0.001) phi = 0.001;
    }
    if (Game::movingCamera){
        GLfloat newX = this->positionToLook.x + -xOffset*sin(theta) + yOffset*cos(theta);
        GLfloat newY = this->positionToLook.z + xOffset*cos(theta) + yOffset*sin(theta) ;
        if (newX > 13) newX = 13;
        if (newX < 0) newX = 0;
        if (newY >15) newY = 15;
        if (newY < 0) newY = 0;
        this->positionToLook = glm::vec3(newX,0,newY);
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

/**
 *  Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
 */
void Camera::processMouseScroll(GLfloat yOffset) {
    this->distance -= yOffset;
    if (this->distance < 4.0f) {
        this->distance = 4.0f;
    }
    if (this->distance > 40.0f) {
        this->distance = 40.0f;
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
    this->theta = camera.theta;
    this->phi = camera.phi;
    this->distance = camera.distance;
    this->positionToLook = camera.positionToLook;
    this->updateCameraVectors();
}

/* PRIVATE METHODS */

/**
 * Calculates the front vector from the Camera's Eular angles
 */
void Camera::updateCameraVectors() {
    this->position.x = this->positionToLook.x + sin(phi) * cos(theta) * distance;
    this->position.y = this->positionToLook.y + cos(phi) * distance;
    this->position.z = this->positionToLook.z + sin(phi) * sin(theta) * distance;

    // Calculate the new front vector
    glm::vec3 front = this->positionToLook - this->position;
    this->front = glm::normalize(front);

    // Also re-calculate right and up vectors
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
