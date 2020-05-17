/*
Semestre 2020-2
Proyecto final
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <thread>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"
#include "Sound.h"
#include "Keyframe.h"
const float toRadians = 3.14159265f / 180.0f;
float movOffset;
bool sentido_puerta_refri = false;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;


Texture plainTexture;

//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Sound *music = new Sound(0.0f, 0.0f, 0.0f);

Skybox skybox;
Skybox skybox_noche;

Model *Mundo;
Model *Faro;
Model *Edificio;
Model *Puerta;
Model *Cuartos;
Model *Cajon;
Model *Espejo;
Model *Sillon;
Model *Puerta_refri;
Model *Libro;
Model *Silla_sup;
Model *Silla_inf;

glm::vec3 perimMin(-2.0f,0.0f,-1.8f);
glm::vec3 perimMax(2.4f,9.0f,2.0f);
glm::vec3 posicionCambioDebug;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat rotPuerta = 0.0f;
GLfloat puerta_offset = 25.0f;
GLfloat puerta_refri_anim = 0.0f;
GLfloat animCajon = 0.0f;
GLint dia_flag = 0;
GLboolean luz_faro = false;
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformLightSpaceMatrix = 0, uniformShadowMap = 0;
Keyframe *keyframes_libro = new Keyframe("Keyframes/keyFramesLibro.txt", 60, "Libro");
Keyframe *keyframes_silla = new Keyframe("Keyframes/keyFramesSilla.txt", 60, "Silla");
// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = (unsigned int)i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void loadModelArray(Model objeto, GLfloat* posiciones, glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess, int num_posiciones) {
	glm::vec3 posicion;
	int i_aux = 0;
	posicion = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_posiciones; i++) {
		if (i_aux > 2)
			i_aux = 0;
		if (i_aux == 0) {
			posicion.x = posiciones[i];
		}
		if (i_aux == 1) {
			posicion.y = posiciones[i];
		}
		if (i_aux == 2) {
			posicion.z = posiciones[i];
			model = glm::mat4(1.0);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::translate(model, posicion);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			objeto.RenderModel();
			posicion = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		i_aux++;

	}
}

void loadModelArrayFaro(Model objeto, GLfloat* posiciones, glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess, GLint* ind_point_lights, int num_posiciones) {
	glm::vec3 posicion;
	int i_aux = 0;
	int iL = 0;
	int iL2 = 0;
	posicion = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_posiciones; i++) {
		if (i_aux > 2)
			i_aux = 0;
		if (i_aux == 0) {
			posicion.x = posiciones[i];
		}
		if (i_aux == 1) {
			posicion.y = posiciones[i];
		}
		if (i_aux == 2) {
			iL = ind_point_lights[iL2];
			posicion.z = posiciones[i];
			model = glm::mat4(1.0);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::translate(model, posicion);
			pointLights[iL].SetPosition(posicion.x, 2.8f, posicion.z);
			if(luz_faro)
				pointLights[iL].SetColor(1.0f, 1.0f, 1.0f);
			else
				pointLights[iL].SetColor(0.0f, 0.0f, 0.0f);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			objeto.RenderModel();
			posicion = glm::vec3(0.0f, 0.0f, 0.0f);
			iL2++;
		}
		i_aux++;
	}
}

void loadModelArbustoArray(std::vector<Mesh*> meslist, GLfloat* posiciones, glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess, int num_posiciones) {
	glm::vec3 posicion;
	int i_aux = 0;
	posicion = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_posiciones; i++) {
		if (i_aux > 2)
			i_aux = 0;
		if (i_aux == 0) {
			posicion.x = posiciones[i];
		}
		if (i_aux == 1) {
			posicion.y = posiciones[i];
		}
		if (i_aux == 2) {
			posicion.z = posiciones[i];
			model = glm::mat4(1.0);
			model = glm::translate(model, posicion);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//blending: transparencia o traslucidez
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meslist[3]->RenderMesh();
			glDisable(GL_BLEND);
			posicion = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		i_aux++;
	}
}

void loadModel(Model *model, const char *path) {
	model->LoadModel(path);
}

/**
 * @brief Animacion simple
 *
 * @param pos Posicion a modificar
 * @param pos_final Posicion final
 * @param offset Paso de movimiento
 * @param FX Datos del archivo de efecto de sonido
 * @param status_FX Status de efecto de sonido, evita que se reproduzca mas veces de lo necesario
 */
void animacion_simple(GLfloat *pos, GLfloat pos_inic, GLfloat pos_final, GLfloat offset, const char* FX, bool *status_FX) {
	if (FX != NULL && status_FX != NULL) {
		if (!*status_FX) {
			music->playFX(FX);
			*status_FX = true;
		}
	}
	if (pos_final <= pos_inic) {
		if (*pos > pos_final)
			*pos -= offset * deltaTime;
	}
	else {
		if (*pos < pos_final)
			*pos += offset * deltaTime;
	}

}

/**
 * @brief Rotacion continua
 *
 * @param anim_rot Valor a modificar
 * @param sentido Sentido de la rotacion, solo para uso interno
 * @param min_rot Minima rotacion
 * @param max_rot Maxima rotacion
 * @param offset Paso de rotacion
 */
void rotacion_compleja_anim(GLfloat *anim_rot, bool *sentido, GLfloat min_rot, GLfloat max_rot, GLfloat offset) {
	if (sentido != NULL) {
		if (*anim_rot <= max_rot && *sentido) {
			*anim_rot += offset * deltaTime;
		}
		else if (*anim_rot >= min_rot && !*sentido) {
			*anim_rot -= offset * deltaTime;
		}
		else {
			if (*sentido) {
				*sentido = false;
				*anim_rot = max_rot;
			}
			else {
				*sentido = true;
				*anim_rot = min_rot;
			}
		}
	}
	else {
		if (max_rot > min_rot) {
			if (*anim_rot <= max_rot) {
				*anim_rot += offset * deltaTime;
			}
			else {
				*anim_rot = min_rot;
			}
		}
		else {
			if (*anim_rot <= max_rot) {
				*anim_rot -= offset * deltaTime;
			}
			else {
				*anim_rot = max_rot;
			}
		}

	}
}

GLboolean isInPerimeter(glm::vec3 posAct, glm::vec3 posMin, glm::vec3 posMax){
	if(posAct.x <= posMax.x && posAct.x >= posMin.x && 
		posAct.y <= posMax.y && posAct.y >= posMin.y && 
		posAct.z <= posMax.z && posAct.z >= posMin.z){
			return true;

	}
	return false;
}

void renderScene(Shader *shader) {
	uniformModel = shader->GetModelLocation();
	GLfloat posiciones_faros[] = {
	3.0f, 0.0f,-3.5f,
	3.0f, 0.0f, 4.0f,
	-3.0f, 0.0f,-3.5f,
	-3.0f, 0.0f, 4.0f,
	};
	int num_posiciones_faros = sizeof(posiciones_faros) / sizeof(posiciones_faros[0]);
	GLint inds_luz_faro[] = {
		1,2,3,4
	};
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Mundo->RenderModel();

	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Edificio->RenderModel();

	model = glm::mat4(1.0);
	modelaux = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.45f));
	model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	model = modelaux;
	//model = glm::translate(model, posicionCambioDebug);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.30f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Puerta->RenderModel();
	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Cuartos->RenderModel(true);
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(animCajon, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Cajon->RenderModel();
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Espejo->RenderModel(true);
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Sillon->RenderModel();
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-1.601675f, 2.555f, 0.475860f));
	model = glm::translate(model, glm::vec3(keyframes_libro->getVal("movX"), keyframes_libro->getVal("movY"), keyframes_libro->getVal("movZ")));
	model = glm::rotate(model, glm::radians(keyframes_libro->getVal("giroZ")), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(keyframes_libro->getVal("giroY")), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Libro->RenderModel();
	model = glm::mat4(1.0);
	//model = glm::translate(model, glm::vec3(mainWindow.getCambioX(), mainWindow.getCambioY(), mainWindow.getCambioZ()));
	model = glm::translate(model, glm::vec3(-1.53f, 4.91f, -1.39f));
	model = glm::rotate(model, glm::radians(puerta_refri_anim), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Puerta_refri->RenderModel();
	model = glm::mat4(1.0);
	modelaux = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(keyframes_silla->getVal("movX"),keyframes_silla->getVal("movY"), keyframes_silla->getVal("movZ")));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Silla_inf->RenderModel();
	model = modelaux;
	model = glm::rotate(model, glm::radians(keyframes_silla->getVal("giroY")), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Silla_sup->RenderModel();
	loadModelArrayFaro(*Faro, posiciones_faros, model, uniformModel, uniformSpecularIntensity, uniformShininess, inds_luz_faro, num_posiciones_faros);
}


int main()
{
	mainWindow = Window(1920, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateShaders();
	camera = Camera(glm::vec3(10.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f, 5.0f, 0.5f);
	glm::mat4 lightSpaceMatrix;
	glm::mat4 lightProjection;
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	std::vector<std::thread> threads;
	Mundo = new Model(); 
	Mundo->LoadModel("Models/mundo.obj");
	Faro = new Model();
	Faro->LoadModel("Models/faro.obj");
	Edificio = new Model();
	Edificio->LoadModel("Models/facha_principal.obj");
	Puerta = new Model();
	Puerta->LoadModel("Models/puerta.obj");
	Cuartos = new Model();
	Cuartos->LoadModel("Models/Cuartos.obj");
	Cajon = new Model();
	Cajon->LoadModel("Models/cajon.obj");
	Espejo = new Model();
	Espejo->LoadModel("Models/Espejo.obj");
	Sillon = new Model();
	Sillon->LoadModel("Models/Sillon.obj");
	Libro = new Model();
	Libro->LoadModel("Models/Libro.obj");
	Puerta_refri = new Model();
	Puerta_refri->LoadModel("Models/Puerta_refri.obj");
	Silla_sup = new Model();
	Silla_sup->LoadModel("Models/Silla_sup.obj");
	Silla_inf = new Model();
	Silla_inf->LoadModel("Models/Silla_inf.obj");
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	
	// luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[2] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[3] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[4] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[5] = PointLight(0.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		3.4f, 5.5f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[6] = PointLight(0.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		3.4f, 3.7f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[7] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[8] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, -0.5f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/hill_lf.png");
	skyboxFaces.push_back("Textures/Skybox/hill_rt.png");
	skyboxFaces.push_back("Textures/Skybox/hill_dn.png");
	skyboxFaces.push_back("Textures/Skybox/hill_up.png");
	skyboxFaces.push_back("Textures/Skybox/hill_bk.png");
	skyboxFaces.push_back("Textures/Skybox/hill_ft.png");

	std::vector<std::string> skyboxFaces_noche;
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_lf.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_rt.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_dn.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_up.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_bk.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_ft.png");

	skybox = Skybox(skyboxFaces);
	skybox_noche = Skybox(skyboxFaces_noche);

	
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	GLboolean cambioCamara = true;
	//music.playMusic("sound/gorillaz.mp3");
	// Ciclos
	GLint ciclos = 0;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){
		GLfloat now = float(glfwGetTime());
		deltaTime = 1.0f * (now - lastTime);
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		posicionCambioDebug = glm::vec3(mainWindow.getCambioX(), mainWindow.getCambioY(), mainWindow.getCambioZ());
		// Camara en pausa
		if (!mainWindow.getPauseCamera()) {
			if (mainWindow.getCamara() == 0) {
				if (cambioCamara) {
					camera.setCameraPosition(glm::vec3(10.0f, 2.0f, 0.0f));
					cambioCamara = false;
				}
				camera.setLimitPosition(glm::vec3(-16.0f, 0.0f, -16.0f), glm::vec3(16.0f, 16.0f, 16.0f));
				camera.keyControl(mainWindow.getsKeys(), deltaTime);
				camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			}
		} 
		if(mainWindow.getAnimPuerta()) {
			animacion_simple(&rotPuerta, 0.0f, 90.0f, puerta_offset, NULL, NULL);
		}
		else {
			animacion_simple(&rotPuerta, 90.0f, 0.0f, puerta_offset, NULL, NULL);
		}
		if (mainWindow.getAnimCajon()) {
			animacion_simple(&animCajon, 0.0f, 0.20f, 1.0f , NULL, NULL);
		}
		else {
			animacion_simple(&animCajon,0.25f, 0.0f, 1.0f, NULL, NULL);
		}
		if(mainWindow.getAnimPuertaRefri())
			rotacion_compleja_anim(&puerta_refri_anim, &sentido_puerta_refri, 0.0f, 45.0f, 30.0f);
		keyframes_libro->inputKeyframes(mainWindow.getAnimLibro());
		keyframes_libro->animate();
		keyframes_silla->inputKeyframes(mainWindow.getAnimSilla());
		keyframes_silla->animate();
		mainWindow.setAnimLibro(false);
		mainWindow.setAnimSilla(false);
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (int(now) % 10 == 0) {
			if (int(now) != ciclos) {
				dia_flag = !dia_flag;
				ciclos = int(now);
			}
		}
		if (dia_flag == 1) {
			mainLight.setIntensity(0.3f, 0.3f);
			pointLights[5].SetColor(0.0f, 0.0f, 0.0f);
			pointLights[6].SetColor(0.0f, 0.0f, 0.0f);
			luz_faro = false;
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			mainLight.setIntensity(0.0, 0.0);
			pointLights[5].SetColor(1.0f, 1.0f, 0.5f);
			pointLights[6].SetColor(1.0f, 1.0f, 0.5f);
			luz_faro = true;
			skybox_noche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		if(isInPerimeter(camera.getCameraPosition(), perimMin, perimMax)){
			luz_faro = false;
			mainLight.setIntensity(0.2f, 0.2f);
			pointLights[0].SetColor(1.0f, 0.5f, 0.1f);
			pointLights[5].SetColor(0.0f, 0.0f, 0.0f);
			pointLights[6].SetColor(0.0f, 0.0f, 0.0f);
			pointLights[7].SetColor(0.0f, 0.95f, 1.0f);
			pointLights[7].SetPosition(2.2f, 7.3f, -0.5f);
			pointLights[8].SetColor(1.0f, 0.541f, 0.862f);
			pointLights[8].SetPosition(1.7f, 7.5f, 1.1f);
			pointLights[8].setIntensity(0.0f, 1.0f);
		}else{
			pointLights[7].SetColor(0.0f, 0.0f, 0.0f);
			pointLights[8].SetColor(0.0f, 0.0f, 0.0f);
		}
		lightProjection = projection;
		lightSpaceMatrix = lightProjection * camera.calculateViewMatrix();
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformShadowMap = shaderList[0].GetShadowMapLocation();
		uniformLightSpaceMatrix = shaderList[0].GetLightSpaceMatrixLocation();
		glUniform1i(uniformShadowMap,1);
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		if(mainWindow.OnOffLight())
			spotLights[0].SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		else
			spotLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		printf("%ff %ff %ff\n",camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		printf("Debug: %ff %ff %ff\n",posicionCambioDebug.x,posicionCambioDebug.y,posicionCambioDebug.z);
		renderScene(&shaderList[0]);			
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}