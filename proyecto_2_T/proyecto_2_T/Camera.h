#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <glfw3.h>
/**
 * @brief Clase camara
 * 
 */
class Camera
{
public:
	/**
	 * @brief Construye un nuevo objeto de camara
	 * 
	 */
	Camera();
	/**
	 * @brief Construye un camara recibiendo parametros
	 * 
	 * @param startPosition Posicion de inicio
	 * @param startUp Up
	 * @param startYaw Yaw
	 * @param startPitch Pitch
	 * @param startMoveSpeed Velocidad inicial de movimiento
	 * @param startTurnSpeed Velocidad inicial de rotacion
	 */
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	/**
	 * @brief Control de teclas para la camara
	 * Controla la posicion de la camara
	 * @param keys Arreglo de status del teclado
	 * @param deltaTime cambio de tiempo
	 */
	void keyControl(bool* keys, GLfloat deltaTime);
	/**
	 * @brief Control del mouse
	 * Controla la rotacion de la camara
	 * @param xChange Cambio del mouse en el eje x del viewport
	 * @param yChange Cambio del mouse en el eje y del viewport
	 */
	void mouseControl(GLfloat xChange, GLfloat yChange);
	/**
	 * @brief Get the Camera Position object
	 * 
	 * @return glm::vec3 Vector con la posicion de la camara
	 */
	glm::vec3 getCameraPosition();
	/**
	 * @brief Set the Limit Position object
	 * Pone los limites de la camara respecto a su posicion
	 * @param lim_min Vector con los limites minimos
	 * @param lim_max Vector con los limites maximos
	 */
	void setLimitPosition(glm::vec3 lim_min, glm::vec3 lim_max);
	/**
	 * @brief Set the Camera Position object
	 * Pone una posicion predeterminada a la camara
	 * @param pos Vector de posicion de la camara
	 */
	void setCameraPosition(glm::vec3 pos) { position = pos; };
	/**
	 * @brief Get the Camera Direction object
	 * Retorna la direccion de vision de la camra
	 * @return glm::vec3 
	 */
	glm::vec3 getCameraDirection();
	/**
	 * @brief Calula la matriz de view
	 * 
	 * @return glm::mat4 
	 */
	glm::mat4 calculateViewMatrix();
	
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	GLfloat max_x, min_x, max_y, min_y, max_z, min_z;
	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

