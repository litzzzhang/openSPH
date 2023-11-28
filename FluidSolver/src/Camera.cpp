#include "Camera.h"


Camera::Camera(glm::vec3 position /*= glm::vec3(0.0f)*/, glm::vec3 up /*= glm::vec3(0.0f, 1.0f, 0.0f)*/, float yaw /*= YAW*/, float pitch /*= PITCH*/)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)), camSpeed_(SPEED), mouseSensitivity_(SENSITIVITY), zoom_(ZOOM)
{
	position_ = position;
	worldup_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMat()
{
	return glm::lookAt(position_, front_ + position_, up_);
}

void Camera::ProcessKeyboard(Direction direction, float deltaTime)
{
    float velocity = camSpeed_ * deltaTime;
    switch (direction)
    {
    case Direction::FORWARD:
        position_ += front_ * velocity;
        break;
    case Direction::BACKWARD:
        position_ -= front_ * velocity;
        break;
    case Direction::LEFT:
        position_ -= right_ * velocity;
        break;
    case Direction::RIGHT:
        position_ += right_ * velocity;
        break;
    default:
        break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity_;
    yoffset *= mouseSensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f)
        zoom_ = 1.0f;
    if (zoom_ > 45.0f)
        zoom_ = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front = glm::vec3(0.0f);
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // also re-calculate the Right and Up vector
    // normalize the vectors, 
    // because their length gets closer to 0 the more you look up or down which results in slower movement.
    right_ = glm::normalize(glm::cross(front_, worldup_));  
    up_ = glm::normalize(glm::cross(right_, front_));
}
