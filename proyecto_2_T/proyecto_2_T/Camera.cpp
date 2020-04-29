#include "Camera.h"
#include <stdio.h>
Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
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

void Camera::mouseControlAerea(){
	front[0] = 0.0006f;
	front[1] = -1.0f;
	front[2] = 0.034f;

	up[0] = 0.017f;
	up[1] = 0.034f;
	up[2] = 1.0f;

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

void Camera::keyControlAvatar(GLfloat pos_x, GLfloat pos_z) {
	position[0] = pos_x;
	position[1] = 20.0f;
	position[2] = pos_z;
}


void Camera::keyControlAerea(bool* keys, GLfloat deltaTime){
	GLfloat velocity = moveSpeed * deltaTime;

	position[1] = 33.33f; //valor de y


	if (keys[GLFW_KEY_D])
	{
		position[0] += front[1] * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position[0] -= front[1] * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position[2] -= right[2] * velocity * 100.0f;
	}

	if (keys[GLFW_KEY_W])
	{
		position[2] += right[2] * velocity * 100.0f;
	}
}

void Camera::keyControlQuiosco(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = moveSpeed * deltaTime;

	position[2] = -14.0f; //valor de y
	position[1] = 4.5f; //valor de y
	position[0] = -2.0f; //valor de y


	if (keys[GLFW_KEY_D])
	{
		position[0] += front[1] * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position[0] -= front[1] * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position[1] -= right[2] * velocity * 100.0f;
	}

	if (keys[GLFW_KEY_W])
	{
		position[1] += right[2] * velocity * 100.0f;
	}
}

void Camera::mouseControlQuiosco() {
	front[0] = 0.03447f;
	front[1] = -0.156434f;
	front[2] = -0.987f;

	up[0] = 0.00546f;
	up[1] = 0.9876f;
	up[2] = -0.156339f;

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

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
