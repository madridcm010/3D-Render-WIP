#pragma once

#include <vector>

#define GLFW_STATIC

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement{

    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera{
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH ): front( glm::vec3(0.0f, 0.0f, -1.0f)), movementspeed(SPEED),mouseSensitivity(SENSITIVTY), zoom(ZOOM)
        {
            this->position = position;
            this->worldup = up;
            this->yaw = yaw;
            this->pitch = pitch;
            this->UpdateCameraVectors( );
        }

        Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch): front(glm::vec3(0.0f,1.0f,-1.0f)), movementspeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
        {
            this->position = glm::vec3(posX, posY, posZ);
            this->worldup = glm::vec3(upX, upY, upZ);
            this->yaw = yaw;
            this->pitch =pitch;
            this->UpdateCameraVectors( );
        }

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(this->position, this->position + this->front, this->up);
        }

        void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
        {
            GLfloat velocity = this->movementspeed * deltaTime;

            if(FORWARD == direction)
            {
                this->position += this->front * velocity;
            }
            if(BACKWARD == direction)
            {
                this->position -= this->front * velocity;
            }
            if(LEFT == direction)
            {
                this->position -= this->right * velocity;
            }
            if(RIGHT == direction)
            {
                this->position += this->right * velocity;
            }
        }
        
        void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
        {
            xOffset *= this->mouseSensitivity;
            yOffset *= this->mouseSensitivity;

            this->yaw += xOffset;
            this->pitch += yOffset;

            if(constrainPitch)
            {
                if(this->pitch > 89.0f)
                {
                    this->pitch = 89.0f;
                }
                if(this->pitch <-89.0f)
                {
                    this->pitch = -89.0f;
                }
            this->UpdateCameraVectors();
            }
        }

        void ProcessMouseScroll(GLfloat yOffset)
        {
            if(this->zoom >= 1.0f && this-> zoom <= 45.0f)
            {
                this->zoom -= yOffset;
            }
            if(this->zoom <= 1.0f)
            {
                this->zoom = 1.0f;
            }
            if(this->zoom >= 45.0f)
            {
                this->zoom = 45.0f;
            }
        }
        GLfloat GetZoom()
        {
            return this->zoom;
        }
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldup;

        GLfloat yaw;
        GLfloat pitch;
        GLfloat movementspeed;
        GLfloat mouseSensitivity;
        GLfloat zoom;

        void UpdateCameraVectors()
        {
            glm::vec3 front;
            front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            front.y = sin(glm::radians(this->pitch));
            front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            
            this->front = glm::normalize(front);
            this->right = glm::normalize(glm::cross(this->front,this->worldup));
            this->up = glm::normalize(glm::cross(this->right,this->front));
        }
};      
