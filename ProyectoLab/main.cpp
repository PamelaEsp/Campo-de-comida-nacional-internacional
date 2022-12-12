/*
Semestre 2023-1
Animación:
1.- Simple o básica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
4.- Técnicas de Animación: Por Keyframes
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>r
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <chrono>  // for high_resolution_clock
#include "addons.h"

// Models
#include "Tux_M.h"

//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
const float FLOOR_SIZE = 60.0f;

// PROJECT SETUP
const int DAY_DURATION = 5; // Seconds
std::chrono::steady_clock::time_point start_time;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetu = 0.0f;
float toffsetv = 0.0f;
//float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

Window mainWindow;
Camera camera;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture sign2x1Texture;
Texture signDescuentoTexture;

Mesh* pisoMesh;
Mesh* signMesh;

//food trucks :
Model ItalianTruck;
Model AssiaticTruck;
Model AmericanTruck;
Model ArabicTruck;

//Comida Mexicana :
Model tortas; 
Model tacos;
Model puesto_garnachas;
Model puesto_aguas;

//Modelos extra
Model mesa;
Model banio;
Model lampara;
Model cartel;

Model Stage_M;
Model StageLight_M;  // Reused 3 times 

Skybox skybox;
std::vector<std::string> skyboxDayFaces, skyboxNightFaces;


//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight sunLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	/* |===========|
	*  |    TEXT   |
	*  |===========|
	*/
	unsigned int letreroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat letreroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	// 0
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,  // 1
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,  // 2
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,  // 3
	};

	pisoMesh = new Mesh();
	pisoMesh->CreateMesh(floorVertices, floorIndices, 32, 6);

	signMesh = new Mesh();
	signMesh->CreateMesh(letreroVertices, letreroIndices, 32, 6);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


//contador de luces puntuales lo declaramos aqui para que podamos prenderlas y apagarlas con el ciclod de los skybox
unsigned int pointLightCount = 0;

bool checkTime(std::chrono::steady_clock::time_point start, bool day_state) {
	// Revisamos si el número de segundos ya supero el limite
	float diff = getSecondsDiff(start);
	//printf("The diff is %i\n", diff);

	if (diff >= DAY_DURATION)
	{
		if (day_state)
		{
			//skybox = Skybox(skyboxNightFaces);
			pointLightCount += 3;//si es de noche encendemos las 3 
		}
		else {
			//skybox = Skybox(skyboxDayFaces);
			pointLightCount = 0;//si es de dia las apagamos
		}
		return true;
	}
	return false;
}

void checkForLightThreeshold(SpotLight lights[], glm::vec3 COLORS[], std::chrono::steady_clock::time_point& start) {

	// Revisar el limite de tiempo
	float diff = getSecondsDiff(start);

	if (diff >= 2) {
		for (int i = 0; i < 3; i++) {
			int index = rand() % 3 + 0;
			lights[i].setColor(COLORS[index].x, COLORS[index].y, COLORS[index].z);
		}
	}

	start = std::chrono::high_resolution_clock::now();
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	pisoTexture = Texture("Textures/piso_pasto_skybox2.tga");
	pisoTexture.LoadTextureA();

	sign2x1Texture = Texture("Textures/letrero_2x1.tga");
	sign2x1Texture.LoadTextureA();

	signDescuentoTexture = Texture("Textures/letrero_descuento.tga");
	signDescuentoTexture.LoadTextureA();

	//--------------------------------------------------Cargando Modelos----------------------------------------------------------------------------

	AssiaticTruck = Model();
	AssiaticTruck.LoadModel("Models/asian/asian_truck.obj");

	AmericanTruck = Model();
	AmericanTruck.LoadModel("Models/hotdog/hotdog_ham.obj");

	ItalianTruck = Model();
	ItalianTruck.LoadModel("Models/italiana/italian_truck.obj");

	ArabicTruck = Model();
	ArabicTruck.LoadModel("Models/arabic/arabic.obj");

	//Para puestos de comida reginal
	tacos = Model();
	tacos.LoadModel("Models/Puesto_tacos_pastor/puesto_tacos_pastor.obj");

	tortas = Model();
	tortas.LoadModel("Models/Puesto_tortas/Puesto_tortas_textura.obj");

	puesto_garnachas = Model();
	puesto_garnachas.LoadModel("Models/puesto_garnachas/puesto_garanachas.obj");

	puesto_aguas = Model();
	puesto_aguas.LoadModel("Models/puesto_Aguas_Frescas/puesto_aguas_frescas_v2.obj");

	mesa = Model();
	mesa.LoadModel("Models/mesas/mesa.obj");


	banio = Model();
	banio.LoadModel("Models/banio/banio3.obj");


	Stage_M = Model();
	Stage_M.LoadModel("Models/stage_clean.obj");
	StageLight_M = Model();
	StageLight_M.LoadModel("Models/stage_light_2.obj");


	lampara = Model();
	lampara.LoadModel("Models/lampara/lampara.obj");

	cartel = Model();
	cartel.LoadModel("Models/cartel/Anuncion_neon.obj");

	Tux_M tux = Tux_M(
		glm::vec3(0.0f),
		"Models/tux_v2/Cuerpo.obj", // body
		"Models/tux_v2/brazo_i.obj",
		"Models/tux_v2/brazo_d.obj",
		"Models/tux_v2/pie_i.obj",
		"Models/tux_v2/pie_d.obj"
	);

	skyboxDayFaces.push_back("Textures/Skybox/dia/dia_lt.tga");
	skyboxDayFaces.push_back("Textures/Skybox/dia/dia_rt.tga");
	skyboxDayFaces.push_back("Textures/Skybox/dia/dia_dn.tga");
	skyboxDayFaces.push_back("Textures/Skybox/dia/dia_up.tga");
	skyboxDayFaces.push_back("Textures/Skybox/dia/dia_bk.tga");
	skyboxDayFaces.push_back("Textures/Skybox/dia/dia_ft.tga");

	skyboxNightFaces.push_back("Textures/Skybox/noche/lt.tga");
	skyboxNightFaces.push_back("Textures/Skybox/noche/rt.tga");
	skyboxNightFaces.push_back("Textures/Skybox/noche/down.tga");
	skyboxNightFaces.push_back("Textures/Skybox/noche/up.tga");
	skyboxNightFaces.push_back("Textures/Skybox/noche/back.tga");
	skyboxNightFaces.push_back("Textures/Skybox/noche/front.tga");

	skybox = Skybox(skyboxDayFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	glm::vec3 COLORS[4];
	COLORS[0] = glm::vec3(1.0f, 0.0f, 0.0f);  // RED
	COLORS[1] = glm::vec3(0.0f, 1.0f, 0.0f);  // GREEN
	COLORS[2] = glm::vec3(0.0f, 0.0f, 1.0f);  // BLUE
	COLORS[3] = glm::vec3(1.0f, 1.0f, 0.0f);  // RED + GREEN

	glm::vec2 CIRCLE[4];
	CIRCLE[0] = glm::vec2(1.0f, 0.0f);
	CIRCLE[1] = glm::vec2(0.0f, 1.0f);
	CIRCLE[2] = glm::vec2(-1.0f, 0.0f);
	CIRCLE[3] = glm::vec2(0.0f, -1.0f);

	float stageRot = 45.0f;
	int circleIndex = 0;

	//luz direccional, sólo 1 y siempre debe de existir
	// ### SUN Light ###
	sunLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,	// COLOR
		0.8f, 0.3f,			// AmbientIntensity / DifusseIntensity
		0.0f, 0.0f, -1.0f); // Direction


	/* TODO: Cada farola tendra un pointLight que debera encender y apagarse*/
	//float aIntensity = getAttenuationValue(1.0f, 0.014f, 0.0007f, 325.0f);

	unsigned int pointLightCount = 0;
	// Farola de sushi y hotdog
	pointLights[0] = PointLight(
		1.0f, 1.0f, 0.4f,	// R G B
		//1.0f, 0.0f, 0.0f,
		100.0f, 1.0f,	// AmbientIntenssity / diffuseIntensity
		100.0f, 6.0f, 140.0f,	// Position
		0.5f, 0.7f, 0.1f);	// Contant, linar, exponent
	pointLightCount++;

	// Farola de italiana y arabe
	pointLights[1] = PointLight(
		1.0f, 1.0f, 0.4f,	// R G B
		100.0f, 1.0f,	// AmbientIntenssity / diffuseIntensity
		100.0f, 6.0f, -150.0f,	// Position
		0.5f, 0.7f, 0.1f);	// Contant, linar, exponent
	pointLightCount++;

	// Farola de tacos y aguas
	pointLights[2] = PointLight(
		1.0f, 1.0f, 0.4f,	// R G B
		100.0f, 1.0f,	// AmbientIntenssity / diffuseIntensity
		-100.0f, 6.0f, -60.0f,	// Position
		0.5f, 0.7f, 0.1f);	// Contant, linar, exponent
	pointLightCount++;

	
	unsigned int spotLightCount = 0;
	// STAGE LIGHT
	spotLights[0] = SpotLight(
		COLORS[0].x, COLORS[0].y, COLORS[0].z,	// R G B
		1000.0f, 10.0f,							// aIntensity dIntensity
		0.0f, 50.0f, 0.0f,						// XPos, yPos, zPos
		0.0f, -1.0f, 0.0f,						// Direction vector
		1.0f, 0.7f, 0.1f,						// What is this?			
		50.0f);									// Edge
	spotLightCount++;

	spotLights[1] = SpotLight(
		COLORS[2].x, COLORS[2].y, COLORS[2].z,	// R G B
		1000.0f, 10.0f,							// aIntensity dIntensity
		0.0f, 50.0f, 0.0f,						// XPos, yPos, zPos
		0.0f, -1.0f, 0.0f,						// Direction vector
		1.0f, 0.7f, 0.1f,						// What is this?			
		50.0f);									// Edge
	spotLightCount++;

	spotLights[2] = SpotLight(
		COLORS[3].x, COLORS[3].y, COLORS[3].z,	// R G B
		1000.0f, 10.0f,							// aIntensity dIntensity
		0.0f, 50.0f, 0.0f,						// XPos, yPos, zPos
		0.0f, -1.0f, 0.0f,						// Direction vector
		1.0f, 0.7f, 0.1f,						// What is this?			
		50.0f);									// Edge
	spotLightCount++;

	/* 
		- CAMBIAR COLOR CADA 2s
		- Seguir función senoidal para las rotaciones
	*/


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(mainWindow.getBufferWidth() / mainWindow.getBufferHeight()), 0.1f, 1000.0f);

	start_time = std::chrono::high_resolution_clock::now();

	bool prev_day_state = true;
	bool is_day = true;
	bool animateStage = false;

	// Stage Lights
	glm::vec3 l1, l2, l3;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
		prev_day_state = mainWindow.isDay();
		

		// Revisa3 si es de dia o de noche
		if (checkTime(start_time, mainWindow.isDay()))
		{
			start_time = std::chrono::high_resolution_clock::now();
			mainWindow.toggleDay();
		}

		is_day = mainWindow.isDay();

		// ### Day && Night Mode ###
		if (mainWindow.isDay())
		{
			sunLight.setAmbientIntenssity(0.8f);
			//skybox = Skybox(skyboxDayFaces);  // NOTE: Enabling this lags the simulation

			if (prev_day_state != is_day) {
				skybox = Skybox(skyboxDayFaces);
			}

			// TODO: Solo cambiarlas cuando se hace el cambio de dia a noche
			// Enable lamps
			for (auto& light : pointLights) {
				light.setIntensity(0.0f);
			}

			
		}
		else {
			sunLight.setAmbientIntenssity(0.35f);
			
			if (prev_day_state != is_day) {
				skybox = Skybox(skyboxNightFaces);
			}

			// Disable lamps
			for (auto& light : pointLights) {
				light.setIntensity(100.0f);
			}

		}

		bool* keys = mainWindow.getsKeys();
		bool prev_key_nine = keys[GLFW_KEY_9];
		//Recibir eventos del usuario
		glfwPollEvents();
		keys = mainWindow.getsKeys();
		
		// TODO: Revisar el modo de camara activo (3rd person or isometric)
		if (mainWindow.getCameraMode()) {
			// 3rd Person Mode
			tux.keyControl(mainWindow.getsKeys(), deltaTime);
			tux.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.adjustCamera(tux.getPos(), tux.getDir(), tux.getWorldUp(), tux.getCAMERA_SCALE());
		}
		else {
			// Aero Mode
			tux.keyControl(mainWindow.getsKeys(), deltaTime);
			tux.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.adjustAeroCamera(tux.getPos(), tux.getDir());
		}

		// ### Luz de las Farolas ###
		if (keys[GLFW_KEY_B])  // Disable nearest pointlight
		{
			int nearestIndex = getNearestLightIndex(pointLights, pointLightCount, tux.getPos());
			printf("DISABLE LIGHT: Nearest index is: %d\n", nearestIndex);

			if (pointLights[nearestIndex].getIntensity() > 0.0f) {
				pointLights[nearestIndex].setIntensity(0.0f);
			}
		}

		if (keys[GLFW_KEY_V])  // Enable nearest pointlight
		{
			int nearestIndex = getNearestLightIndex(pointLights, pointLightCount, tux.getPos());
			printf("ENABLE LIGHT: Nearest index is: %d\n", nearestIndex);

			if (pointLights[nearestIndex].getIntensity() <= 0.0f) {
				pointLights[nearestIndex].setIntensity(1.0f);
			}
		}


		if (keys[GLFW_KEY_9])  // Enable stage animation
		{
			
			if (prev_key_nine != keys[GLFW_KEY_9]) { 
				animateStage = true;
				printf("CHANGING STAGE LIGHTS\n");
			}
		}


		if (keys[GLFW_KEY_8])  // Disable state animation
		{
			
			if (prev_key_nine != keys[GLFW_KEY_8]) {
				animateStage = false;
				printf("CHANGING STAGE LIGHTS\n");
			}
		}

		// para keyframes
		//inputKeyframes(mainWindow.getsKeys());
		//animate();


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&sunLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		/* 
		*	--------------------------------------------------
		*		CARGA DE MODELOS
		*	--------------------------------------------------
		*/ 

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		// Make plane bigger
		model = glm::scale(model, glm::vec3(FLOOR_SIZE, 1.0f, FLOOR_SIZE));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pisoMesh->RenderMesh();

		model = glm::mat4(1.0);
		glm::vec3 stagePos = glm::vec3(5.0f, 30.0f, 220.0f);
		model = glm::translate(model, glm::vec3(5.0f, -5.0f, 220.0f));
		model = glm::scale(model, glm::vec3(7.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Stage_M.RenderModel();
		
		if (animateStage) {
			int index;

			stageRot += 1.0f;

			// Enable Lights

			spotLights[0].setIntensity(100.0);
			spotLights[1].setIntensity(100.0);
			spotLights[2].setIntensity(100.0);

			spotLights[0].SetPos(stagePos);
			spotLights[1].SetPos(stagePos);
			spotLights[2].SetPos(stagePos);


			l1 = glm::vec3(
				1 * glm::sin(-stageRot * toRadians) * stagePos.x,
				glm::sin(-stageRot * toRadians) - stagePos.y,
				1 * glm::cos(stageRot * toRadians) * stagePos.z
			);

			l2 = glm::vec3(
				1000 * glm::sin(stageRot * toRadians) * stagePos.x + 50,
				glm::sin(stageRot * toRadians) - stagePos.y,
				glm::cos(stageRot * toRadians) * stagePos.z + 50
			);

			l3 = glm::vec3(
				500 * glm::sin(-stageRot * toRadians) * stagePos.x - 75,
				glm::sin(-stageRot * toRadians) - stagePos.y,
				glm::cos(-stageRot * toRadians) * stagePos.z - 75
			);

			l1 = glm::normalize(l1);
			l2 = glm::normalize(l2);
			l3 = glm::normalize(l3);

			spotLights[0].setDirection(l1);
			spotLights[1].setDirection(l2);
			spotLights[2].setDirection(l3);

			for (auto& light : spotLights) {
				index = rand() % 3 + 0;
				// TODO: Check if color is asssigned already, if it is, choose another one 

				light.setColor(COLORS[index].x, COLORS[index].y, COLORS[index].z);
				
			}
		}
		else {
			// Turn off the lights!
			spotLights[0].setIntensity(0);
			spotLights[1].setIntensity(0);
			spotLights[2].setIntensity(0);
		}



		// Light 1
		glm::mat4 aux = model;
		model = glm::rotate(model, stageRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		StageLight_M.RenderModel();

		// Light 2
		//model = glm::mat4(1.0);
		aux = model;
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, stageRot * toRadians + 50, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(100.0f, 100.f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		StageLight_M.RenderModel();

		// Light 3
		model = aux;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, stageRot * toRadians + 10, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(100.0f, 100.f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		StageLight_M.RenderModel();

		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -5.0f, -600.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		*/

		//----------------------------------------------------------Areas de comida------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f,1.0f,210.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AssiaticTruck.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 1.0f, 70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AmericanTruck.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 1.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ItalianTruck.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 1.0f, -220.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArabicTruck.RenderModel();

		//----------------------------------------------------------Areas de comida------------------------------------------------------------------

		//Puesto de tortas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, -0.5f, 150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tortas.RenderModel();

		//Puesto de tacos 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, 50.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 5.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tacos.RenderModel();

		//Puesto de aguas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -130.0f));
		model = glm::scale(model, glm::vec3(2.5f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puesto_aguas.RenderModel();

		//Puesto de garnachas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -250.0f));
		model = glm::scale(model, glm::vec3(5.0f, 10.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puesto_garnachas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -140.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -140.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa.RenderModel();



		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -350.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -350.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -350.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		//-----------------------------------------------------------------------Modelos de laparas----------------------
		//Lado de food trucks
		//Entre sushi y hot dog 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, 140.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//Entre hot dog e italiana 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//Entre italiana y arabe
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//Lado de comida mexicana 
		//Entre tortas y tacos 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, 80.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//Entre tacos y aguas 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -60.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//Entre púesto de aguas y garnchas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -200.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//-------------------------------------PARA CARTELES
		//Entre púesto de aguas y garnchas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -180.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cartel.RenderModel();

		//Entre foodtrucks
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cartel.RenderModel();

		tux.move(uniformModel);

		// Carteles
		toffsetu += 0.001;
		toffsetv += 0.0;

		if (toffsetu > 1.0)
			toffsetu = 0.0;

		toffset = glm::vec2(toffsetu, toffsetv);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-97.8f, 26.0f, -179.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 13.0f, 7.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sign2x1Texture.UseTexture();
		signMesh->RenderMesh();

		// Cartel FoodTrucks
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(95.0f, 26.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 13.0f, 7.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		signDescuentoTexture.UseTexture();
		signMesh->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}