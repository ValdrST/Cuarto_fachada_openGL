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
float movCoche;
float movOffset;
bool avanza;
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

Sound music;

Skybox skybox;
Skybox skybox_noche;

Model *Mundo;
Model *Faro;
Model *Edificio;
Model *Puerta;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLint dia_flag = 0;
Shader *shader2;
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformLightSpaceMatrix = 0, uniformShadowMap = 0;
// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
static const char* vShader2 = "shaders/shader_depth.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
static const char* fShader2 = "shaders/shader_depth.frag";


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
		unsigned int nOffset = unsigned int(i) * vLength + normalOffset;
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
	shader2 = new Shader();
	shader2->CreateFromFiles(vShader2, fShader2);

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
			if (dia_flag == 0)
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



void renderScene(Shader *shader){
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
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Puerta->RenderModel();
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
	music = Sound(0.0f, 0.0f, 0.0f);
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
	
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	
	// luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
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
	GLint ciclos = 0;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()){
		GLfloat now = float(glfwGetTime());
		deltaTime = 1.0f * (now - lastTime);
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
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
			if (mainWindow.getCamara() == 1) {
				if (!cambioCamara) {
					camera.setCameraPosition(glm::vec3(10.0f, -40.0f, 0.0f));
					cambioCamara = true;
				}
				camera.setLimitPosition(glm::vec3(-16.0f, -40.0f, -16.0f), glm::vec3(16.0f, -10.0f, 16.0f));
				camera.keyControl(mainWindow.getsKeys(), deltaTime);
				camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			}
		} 

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
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			mainLight.setIntensity(0.0, 0.0);
			pointLights[5].SetColor(1.0f, 1.0f, 0.5f);
			pointLights[6].SetColor(1.0f, 1.0f, 0.5f);
			skybox_noche.DrawSkybox(camera.calculateViewMatrix(), projection);
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
		
		//printf("%f %f %f\n", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);	
		renderScene(&shaderList[0]);			
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}