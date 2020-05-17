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
	animPuerta = false;
	animPuertaRefri = false;
	animCajon = false;
	animLibro = false;
	animSilla = false;
	statusLight = false;
	cambioX = 0.0f;
	cambioY = 0.0f;
	cambioZ = 0.0f;

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 2);

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
	glEnable(GL_MULTISAMPLE);  
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


void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_KP_8){
		theWindow -> cambioX += 0.01f;
	}
	if (key == GLFW_KEY_KP_2){
		theWindow -> cambioX -= 0.01f;
	}
	if (key == GLFW_KEY_KP_6){
		theWindow -> cambioY += 0.01f;
	}
	if (key == GLFW_KEY_KP_4){
		theWindow -> cambioY -= 0.01f;
	}
	if (key == GLFW_KEY_KP_9){
		theWindow -> cambioZ += 0.01f;
	}
	if (key == GLFW_KEY_KP_7){
		theWindow -> cambioZ -= 0.01f;
	}
	if (key == GLFW_KEY_1)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animPuerta == true)
				theWindow->animPuerta = false;
			else
				theWindow->animPuerta = true;
		}
	}
	if (key == GLFW_KEY_2)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animCajon == true)
				theWindow->animCajon = false;
			else
				theWindow->animCajon = true;
		}
	}
	if (key == GLFW_KEY_3)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animLibro == true)
				theWindow->animLibro = false;
			else
				theWindow->animLibro = true;
		}
	}
	if (key == GLFW_KEY_4)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animPuertaRefri == true)
				theWindow->animPuertaRefri = false;
			else
				theWindow->animPuertaRefri = true;
		}
	}
	if (key == GLFW_KEY_5)
	{
		if (action == GLFW_RELEASE) {
			if (theWindow->animSilla == true)
				theWindow->animSilla = false;
			else
				theWindow->animSilla = true;
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
	if (key == GLFW_KEY_C) {
		if (action == GLFW_RELEASE) {
			if (!theWindow->camara_pausa)
				theWindow->camara++;
			if (theWindow->camara > 0) {
				theWindow->camara = 0;
			}
		}
	}
	if (key == GLFW_KEY_X) {
		if (action == GLFW_RELEASE) {
			theWindow->camara_pausa = !theWindow->camara_pausa;
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
