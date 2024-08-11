// Camera.hpp - Header file for the camera class.
#pragma once

#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL

class Camera {
    public:
        Camera();

        glm::mat4 GetViewMatrix() const;
        glm::vec3 GetEyePosition() const;
        glm::vec3 GetCameraPosition();

        // Move Camera around.
        void MouseLook(int mouseX, int mouseY);
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        void MoveUp(float speed);
        void MoveDown(float speed);

        // Setters and Getters.
        void  SetCameraEyePosition(float x, float y, float z);
        float GetEye_X_Position();
        float GetEye_Y_Position();
        float GetEye_Z_Position();
        float GetView_X_Direction();
        float GetView_Y_Direction();
        float GetView_Z_Direction();

    private: 
        glm::vec2 m_oldMousePosition;
        glm::vec3 m_eyePosition;
        glm::vec3 m_viewDirection;
        glm::vec3 m_upVector;
        glm::vec3 m_rightVector;

        // Prevents shadow copies from being created.
        Camera(const Camera& copy){}
};
