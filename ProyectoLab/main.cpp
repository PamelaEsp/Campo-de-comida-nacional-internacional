/*
Semestre 2023-1
Animaci�n:
1.- Simple o b�sica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
4.- T�cnicas de Animaci�n: Por Keyframes
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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
const float FLOOR_SIZE = 60.0f;

// PROJECT SETUP
const int DAY_DURATION = 10; // Seconds
std::chrono::steady_clock::time_point start_time;



//variables para animaci�n
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetu = 0.0f;
float toffsetv = 0.0f;
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;



Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;

Mesh* pisoMesh;

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
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	//Mesh* obj1 = new Mesh();
	//obj1->CreateMesh(vertices, indices, 32, 12);
	//meshList.push_back(obj1);

	pisoMesh = new Mesh();
	pisoMesh->CreateMesh(floorVertices, floorIndices, 32, 6);

	/*Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);*/

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;


//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 30
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;

	FrameIndex++;
}

void resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se qued� aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/

//contador de luces puntuales lo declaramos aqui para que podamos prenderlas y apagarlas con el ciclod de los skybox
unsigned int pointLightCount = 0;

bool checkTime(std::chrono::steady_clock::time_point start, bool day_state) {
	// Revisamos si el n�mero de segundos ya supero el limite
	float diff = getSecondsDiff(start);
	//printf("The diff is %i\n", diff);

	if (diff >= DAY_DURATION)
	{
		if (day_state)
		{
			skybox = Skybox(skyboxNightFaces);
			pointLightCount += 3;//si es de noche encendemos las 3 
		}
		else {
			skybox = Skybox(skyboxDayFaces);
			pointLightCount = 0;//si es de dia las apagamos 
		}
		return true;
	}
	return false;
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
	banio.LoadModel("Models/banio.obj");


	Stage_M = Model();
	Stage_M.LoadModel("Models/stage_clean.obj");

	lampara = Model();
	lampara.LoadModel("Models/lampara/lampara.obj");

	cartel = Model();
	cartel.LoadModel("Models/cartel/Anuncion_neon.obj");

	Tux_M tux = Tux_M(
		glm::vec3(0.0f),
		"Models/tux/body.obj", // body
		"Models/tux/larm.obj",
		"Models/tux/rarm.obj",
		"Models/tux/lfoot.obj",
		"Models/tux/rfoot.obj"
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


	//luz direccional, s�lo 1 y siempre debe de existir
	// ### SUN Light ###
	sunLight = DirectionalLight(
		1.0f, 1.0f, 1.0f,	// COLOR
		0.8f, 0.3f,			// AmbientIntensity / DifusseIntensity
		0.0f, 0.0f, -1.0f); // Direction


	/* TODO: Cada farola tendra un pointLight que debera encender y apagarse*/
	//float aIntensity = getAttenuationValue(1.0f, 0.014f, 0.0007f, 325.0f);

	//contador de luces puntuales
	//unsigned int pointLightCount = 0;
	// Farola de sushi y hotdog
	pointLights[0] = PointLight(
		1.0f, 1.0f, 0.4f,	// R G B
		//1.0f, 0.0f, 0.0f,
		10.0f, 1.0f,	// AmbientIntenssity / diffuseIntensity
		100.0f, 2.0f, 140.0f,	// Position
		0.5f, 0.7f, 0.1f);	// Contant, linar, exponent
	//pointLightCount++;

	// Farola de italiana y arabe
	pointLights[1] = PointLight(
		1.0f, 1.0f, 0.4f,	// R G B
		//1.0f, 0.0f, 0.0f,
		10.0f, 1.0f,	// AmbientIntenssity / diffuseIntensity
		100.0f, 2.0f, -150.0f,	// Position
		0.5f, 0.7f, 0.1f);	// Contant, linar, exponent
	//pointLightCount++;

	// Farola de tacos y aguas
	pointLights[2] = PointLight(
		1.0f, 1.0f, 0.4f,	// R G B
		//1.0f, 0.0f, 0.0f,
		10.0f, 1.0f,	// AmbientIntenssity / diffuseIntensity
		-100.0f, 2.0f, -60.0f,	// Position
		0.5f, 0.7f, 0.1f);	// Contant, linar, exponent
	//pointLightCount++;

	
	unsigned int spotLightCount = 0;
	//linterna
	/*spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;*/

	//luz fija
	/*spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;*/


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(mainWindow.getBufferWidth() / mainWindow.getBufferHeight()), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.001f ;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	avanza = true;

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	//KEYFRAMES DECLARADOS INICIALES
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 180.0f;


	//Agregar Kefyrame[5] para que el avi�n regrese al inicio
	start_time = std::chrono::high_resolution_clock::now();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Revisar si es de dia o de noche
		if (checkTime(start_time, mainWindow.isDay()))
		{
			start_time = std::chrono::high_resolution_clock::now();
			mainWindow.toggleDay();
		}

		// ### Day && Night Mode ###
		if (mainWindow.isDay())
		{
			// # Day Mode #

			sunLight.setAmbientIntenssity(0.8f);
		}
		else {
			// # Night Mode #
			sunLight.setAmbientIntenssity(0.3f);

		}

		//Recibir eventos del usuario
		glfwPollEvents();

		
		// TODO: Revisar el modo de camara activo (3rd person or isometric)
		if (mainWindow.getCameraMode()) {
			// 3rd Person Mode
			tux.keyControl(mainWindow.getsKeys(), deltaTime);
			tux.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.adjustCamera(tux.getPos(), tux.getDir(), tux.getWorldUp(), tux.getCAMERA_SCALE());
		}
		else {
			// Isometric Mode
			tux.keyControl(mainWindow.getsKeys(), deltaTime);
			tux.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.adjustAeroCamera(tux.getPos(), tux.getDir());

			/*camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());*/
		}

		bool* keys = mainWindow.getsKeys();

		// ### Luz de las Farolas ###
		if (keys[GLFW_KEY_V])  // Disable nearest pointlight
		{
			int nearestIndex = getNearestLightIndex(pointLights, pointLightCount, tux.getPos());
			//printf("Nearest index is: %d\n", nearestIndex);

			if (pointLights[nearestIndex].getIntensity() > 0.0f) {
				pointLights[nearestIndex].setIntensity(0.0f);
			}
		}

		if (keys[GLFW_KEY_B])  // Enable nearest pointlight
		{
			int nearestIndex = getNearestLightIndex(pointLights, pointLightCount, tux.getPos());
			//printf("Nearest index is: %d\n", nearestIndex);

			if (pointLights[nearestIndex].getIntensity() <= 0.0f) {
				pointLights[nearestIndex].setIntensity(1.0f);
			}
		}

		// para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();


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

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//informaci�n al shader de fuentes de iluminaci�n
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
		model = glm::translate(model, glm::vec3(5.0f, -5.0f, 220.0f));
		model = glm::scale(model, glm::vec3(7.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Stage_M.RenderModel();

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
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -200.0f));
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

		//Entre p�esto de aguas y garnchas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -200.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//-------------------------------------PARA CARTELES
		//Entre p�esto de aguas y garnchas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -180.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cartel.RenderModel();

		//Entre p�esto de aguas y garnchas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cartel.RenderModel();

		tux.move(uniformModel);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animaci�n'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf(" \npresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}
