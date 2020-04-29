#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	camara = 0;
	muevex;
	muevey;
	muevez;
	girox;
	giroy;
	giroz;
	animHelicoptero = false;
	animKeyHelicoptero = false;
	animKeyPajaro = false;
	animPuerta1 = false;
	animPuerta2 = false;
	statusLight = false;
	status_luzR = false;
	status_luzG = false;
	status_luzB = false;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Alameda", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
int Window::getCamara() {
	return camara;
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

bool Window::getAnimHelicoptero() {
	return animHelicoptero;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (theWindow->animKeyHelicoptero) {
		theWindow->animKeyHelicoptero = false;
	}
	if (theWindow->animKeyPajaro) {
		theWindow->animKeyPajaro = false;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_K)
	{
		theWindow-> muevex -= 0.10f;
	}
	if (key == GLFW_KEY_SEMICOLON)
	{
		theWindow-> muevex += 0.10f;
	}
	if (key == GLFW_KEY_J)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->status_luzR == true)
				theWindow->status_luzR = false;
			else
				theWindow->status_luzR = true;
		}
	}
	if (key == GLFW_KEY_K)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->status_luzG == true)
				theWindow->status_luzG = false;
			else
				theWindow->status_luzG = true;
		}
	}
	if (key == GLFW_KEY_L)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->status_luzB == true)
				theWindow->status_luzB = false;
			else
				theWindow->status_luzB = true;
		}
	}
	if (key == GLFW_KEY_O)
	{
		theWindow-> muevez += 0.10f;
		if (action == GLFW_RELEASE) {
			if (theWindow->animPuerta2 == true)
				theWindow->animPuerta2 = false;
			else
				theWindow->animPuerta2 = true;
		}
	}
	if (key == GLFW_KEY_P)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animPuerta1 == true)
				theWindow->animPuerta1 = false;
			else
				theWindow->animPuerta1 = true;
		}
	}
	if (key == GLFW_KEY_F)
	{
		if(action == GLFW_RELEASE){
			if((GLboolean)theWindow-> statusLight == true)
				theWindow-> statusLight = false;
			else
				theWindow-> statusLight = true;
		}
	}

	if (key == GLFW_KEY_H)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animHelicoptero == true)
				theWindow->animHelicoptero = false;
			else
				theWindow->animHelicoptero = true;
		}
	}
	if (key == GLFW_KEY_1)
	{
		if (action == GLFW_RELEASE) {
			theWindow->animKeyHelicoptero = true;
		}
	}
	if (key == GLFW_KEY_2)
	{
		if (action == GLFW_RELEASE) {
			theWindow->animKeyPajaro = true;
		}
	}
	if (key == GLFW_KEY_C) {
		if (action == GLFW_RELEASE) {
			theWindow->camara++;
			if (theWindow->camara > 3) {
				theWindow->camara = 0;
			}
		}
	}
	if (key == GLFW_KEY_X) {
		if (action == GLFW_RELEASE) {
			if (theWindow->camara == 4)
				theWindow->camara = 0;
			else
				theWindow->camara = 4;
		}
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = float(xPos);
		theWindow->lastY = float(yPos);
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = (GLfloat)xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - (GLfloat)yPos;

	theWindow->lastX = (GLfloat)xPos;
	theWindow->lastY = (GLfloat)yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
