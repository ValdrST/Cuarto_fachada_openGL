#include "Camera.h"
#include <stdio.h>
Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed){
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	max_x = 17.0f;
	min_x = -16.0f;
	min_z = -16.0f;
	max_z = 16.0f;
	min_y = 0.0f;
	max_y = 15.0f;

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::setLimitPosition(glm::vec3 lim_min, glm::vec3 lim_max) {
	min_x = lim_min.x;
	min_y = lim_min.y;
	min_z = lim_min.z;

	max_x = lim_max.x;
	max_y = lim_max.y;
	max_z = lim_max.z;

}

void Camera::keyControl(bool* keys, GLfloat deltaTime){
	if (position[0] > max_x)
		position[0] = max_x;
	else if (position[0] < min_x)
		position[0] = min_x;
	if (position[1] > max_y)
		position[1] = max_y;
	else if (position[1] < min_y)
		position[1] = min_y;
	if (position[2] > max_z)
		position[2] = max_z;
	else if (position[2] < min_z)
		position[2] = min_z;
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (yaw >= 360.0f) {
		yaw = 0.0f;
	}
	if (yaw <= -360.0f) {
		yaw = 0.0f;
	}
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	//printf("yaw:%f pitch:%f\n",yaw, pitch);
	update();
}

glm::mat4 Camera::calculateViewMatrix(){
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition(){
	return position;
}


glm::vec3 Camera::getCameraDirection(){
	return glm::normalize(front);
}

void Camera::update(){
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	//printf("front:%f %f %fup:%f %f %f\n", front.x, front.y, front.z, up.x, up.y, up.z);
}

Camera::~Camera()
{
}
