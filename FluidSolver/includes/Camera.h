#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include <vector>

enum class Direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	// camera parameters
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 worldup_;

	// euler angles
	float yaw_;
	float pitch_;

	//camera options
	float camSpeed_;
	float mouseSensitivity_;
	float zoom_;

	//initialize the camera with position vector, world up vector and Euler angles
	Camera(glm::vec3 position = glm::vec3(0.0f), 
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	
	// return the view matrix
	glm::mat4 getViewMat();

	void ProcessKeyboard(Direction direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
private:
	void updateCameraVectors();
};
