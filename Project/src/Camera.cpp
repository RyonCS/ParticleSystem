// Camera.cpp - Source file for the camera class.
#include "Camera.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

// Constructor
Camera::Camera(){
    // Position the camera.
    m_eyePosition = glm::vec3(0.0f, 0.0f, 5.0f);
    m_viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
}

// Handles camera based on mouse movement.
void Camera::MouseLook(int mouseX, int mouseY) {
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);

    // Little hack for our mouse look function so we can move our camera.
    static bool firstLook=true;
    if(true == firstLook) {
        firstLook=false;
        m_oldMousePosition = newMousePosition;
    }

    glm::vec2 mouseMovement = (newMousePosition - m_oldMousePosition) * 0.25f;

    // Calculate the yaw and pitch matrices.
    glm::mat4 yawMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mouseMovement.x), m_upVector);
    glm::mat4 pitchMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mouseMovement.y), m_rightVector);

    // Calculate the new view direction based on the yaw and pitch matrices.
    m_viewDirection = glm::normalize(glm::mat3(yawMatrix * pitchMatrix) * m_viewDirection);

    // Set old mouse position.
    m_oldMousePosition = newMousePosition;
}

// Move camera forward on W.
void Camera::MoveForward(float speed) {
    m_eyePosition += speed * m_viewDirection;
}

// Move camera backwards on S.
void Camera::MoveBackward(float speed){
    m_eyePosition -= speed * m_viewDirection;
}

// Move camera left on A.
void Camera::MoveLeft(float speed) {
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
    m_eyePosition -= speed * m_rightVector;
}

// Move camera right on D.
void Camera::MoveRight(float speed){
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
    m_eyePosition += speed * m_rightVector;
}

// Move camera up on UP.
void Camera::MoveUp(float speed) {
    m_eyePosition.y += speed;
}

// Move camera down on DOWN.
void Camera::MoveDown(float speed) {
    m_eyePosition.y -= speed;
}

// Sets the camera's current eye position.
void Camera::SetCameraEyePosition(float x, float y, float z) {
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

float Camera::GetEye_X_Position() {
    return m_eyePosition.x;
}

float Camera::GetEye_Y_Position() {
    return m_eyePosition.y;
}

float Camera::GetEye_Z_Position() {
    return m_eyePosition.z;
}

float Camera::GetView_X_Direction() {
    return m_viewDirection.x;
}

float Camera::GetView_Y_Direction(){
    return m_viewDirection.y;
}

float Camera::GetView_Z_Direction() {
    return m_viewDirection.z;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_eyePosition, m_eyePosition + m_viewDirection, m_upVector);
}

glm::vec3 Camera::GetEyePosition() const {
    return m_eyePosition;
}

glm::vec3 Camera::GetCameraPosition() {
    return glm::vec3(GetEye_X_Position(), GetEye_Y_Position(), GetEye_Z_Position());
}