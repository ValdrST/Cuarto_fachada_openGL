#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
/**
 * @brief Clase Window
 * Maneja lo relativo a la ventana y la entrada de teclado y mouse
 * 
 */
class Window
{
public:
	/**
	 * @brief Construct a new Window object
	 * 
	 */
	Window();
	/**
	 * @brief Construct a new Window object
	 * Construye una nueva ventana y recibe su resolucion
	 * @param windowWidth 
	 * @param windowHeight 
	 */
	Window(GLint windowWidth, GLint windowHeight);
	/**
	 * @brief Inicia la instancia de ventana
	 * 
	 * @return int 
	 */
	int Initialise();
	/**
	 * @brief Get the Buffer Width object
	 * 
	 * @return GLfloat 
	 */
	GLfloat getBufferWidth() { return float(bufferWidth); }
	/**
	 * @brief Get the Buffer Height object
	 * 
	 * @return GLfloat 
	 */
	GLfloat getBufferHeight() { return float(bufferHeight); }
	/**
	 * @brief Obtiene el cambio en X de la posicion del puntero del mouse
	 * 
	 * @return GLfloat 
	 */
	GLfloat getXChange();
	/**
	 * @brief Obtiene el cambio en Y de la posicion del puntero del mouse
	 * 
	 * @return GLfloat 
	 */
	GLfloat getYChange();
	/**
	 * @brief 
	 * 
	 * @return GLfloat 
	 */
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez() { return muevez; }
	bool getAnimPuerta1() { return animPuerta1; }
	bool getAnimPuerta2() { return animPuerta2; }
	bool getAnimKeyHelicoptero() { return animKeyHelicoptero; }
	bool getAnimKeyPajaro() { return animKeyPajaro; }
	void setAnimKeyHelicoptero(bool val) {  animKeyHelicoptero = val; }
	void setAnimKeyPajaro(bool val) { animKeyPajaro = val; }
	int getCamara();
	bool getAnimHelicoptero();
	GLboolean OnOffLight() { return statusLight; }
	GLboolean statusluzR() { return status_luzR; }
	GLboolean statusluzG() { return status_luzG; }
	GLboolean statusluzB() { return status_luzB; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	/**
	 * @brief Obtiene un arreglo del estatus de las teclas
	 * 
	 * @return true tecla pulsada
	 * @return false tecla sin pulsar
	 */
	bool* getsKeys() { return keys; }
	/**
	 * @brief 
	 * 
	 */
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLboolean statusLight;
	GLfloat muevex;
	GLfloat muevey;
	GLfloat muevez;
	GLfloat girox;
	GLfloat giroy;
	GLfloat giroz;
	GLint camara;
	bool status_luzR;
	bool status_luzG;
	bool status_luzB;
	bool animHelicoptero;
	bool animPuerta1;
	bool animPuerta2;
	bool animKeyHelicoptero;
	bool animKeyPajaro;
	bool mouseFirstMoved;
	/**
	 * @brief Maneja el teclado
	 * 
	 * @param window 
	 * @param key 
	 * @param code 
	 * @param action 
	 * @param mode 
	 */
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	/**
	 * @brief Maneja el mouse
	 * 
	 * @param window 
	 * @param xPos 
	 * @param yPos 
	 */
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

