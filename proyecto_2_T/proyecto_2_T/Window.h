#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return float(bufferWidth); }
	GLfloat getBufferHeight() { return float(bufferHeight); }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez() { return muevez; }
	bool getAnimPuerta1() { return animPuerta1; }
	bool getAnimPuerta2() { return animPuerta2; }
	bool getAnimKeyHelicoptero() { return animKeyHelicoptero; }
	bool getAnimKeyPajaro() { return animKeyPajaro; }
	void setAnimKeyHelicoptero(bool val) {  animKeyHelicoptero = val; }
	int getCamara();
	bool getAnimHelicoptero();
	GLboolean OnOffLight() { return statusLight; }
	GLboolean statusluzR() { return status_luzR; }
	GLboolean statusluzG() { return status_luzG; }
	GLboolean statusluzB() { return status_luzB; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
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
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

