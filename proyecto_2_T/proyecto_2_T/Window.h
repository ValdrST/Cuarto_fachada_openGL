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
	 * @brief Get the Anim Puerta object
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getAnimPuerta() { return animPuerta; }
	/**
	 * @brief Get the Anim Puerta Refri object
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getAnimPuertaRefri() { return animPuertaRefri; }
	/**
	 * @brief Get the Anim Cajon object
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getAnimCajon() { return animCajon; }
	/**
	 * @brief Get the Anim Libro object
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getAnimLibro() { return animLibro; }
	/**
	 * @brief Set the Anim Libro object
	 * 
	 * @param val 
	 */
	void setAnimLibro(bool val) {  animLibro = val; }
	/**
	 * @brief Get the Anim Silla object
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getAnimSilla() { return animSilla; }
	/**
	 * @brief Set the Anim Silla object
	 * 
	 * @param val 
	 */
	void setAnimSilla(bool val) {  animSilla = val; }
	/**
	 * @brief Get the Cambio X object
	 * 
	 * @return GLfloat 
	 */
	GLfloat getCambioX() { return cambioX; }
	/**
	 * @brief Get the Cambio Y object
	 * 
	 * @return GLfloat 
	 */
	GLfloat getCambioY() { return cambioY; }
	/**
	 * @brief Get the Cambio Z object
	 * 
	 * @return GLfloat 
	 */
	GLfloat getCambioZ() { return cambioZ; }
	/**
	 * @brief Obtiene el valor de la camara
	 * 
	 * @return int 
	 */
	int getCamara();
	/**
	 * @brief Prende y apaga la luz de la linterna de camara
	 * 
	 * @return GLboolean 
	 */
	GLboolean OnOffLight() { return statusLight; }
	/**
	 * @brief Get the Should Close object
	 * 
	 * @return true 
	 * @return false 
	 */
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
	 * @brief swapbuffer
	 * 
	 */
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	/**
	 * @brief Valor de pausa de la camara
	 * 
	 * @return GLboolean 
	 */
	GLboolean getPauseCamera() { return camara_pausa; }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat cambioX;
	GLfloat cambioY;
	GLfloat cambioZ;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLboolean statusLight;
	GLfloat girox;
	GLfloat giroy;
	GLfloat giroz;
	/**
	 * @brief Variable para manejar el perfil de la camara que se va a visualizar
	 * 
	 */
	GLint camara;
	/**
	 * @brief Variable para pausar la camara
	 * 
	 */
	GLboolean camara_pausa;
	bool animCajon;
	bool animPuerta;
	bool animLibro;
	bool animSilla;
	bool animPuertaRefri;
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

