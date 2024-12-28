#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
class Camera 
{
public:
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    float yaw = -90.0f;
    float pitch = 0.0f;
    float movementSpeed = 0.5f;
    float sensitivity = 0.1f;
    void CameraKeyBoardHandler(int direction) 
    {

        if (direction == GLFW_KEY_W) 
            position += front * movementSpeed;
        if (direction == GLFW_KEY_S) 
            position -= front * movementSpeed;
        if (direction == GLFW_KEY_A) 
            position -= right * movementSpeed;
        if (direction == GLFW_KEY_D) 
            position += right * movementSpeed;
        if (direction == GLFW_KEY_SPACE) 
            position += up * movementSpeed;
        if (direction == GLFW_KEY_LEFT_CONTROL) 
            position -= up * movementSpeed;
    }

    glm::mat4 getViewMatrix() 
    {
        return glm::lookAt(position, position + front, up);
    }
};

