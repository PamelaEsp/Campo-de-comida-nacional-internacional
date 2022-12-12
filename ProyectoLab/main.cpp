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
const int DAY_DURATION = 10; // Seconds
std::chrono::steady_clock::time_point start_time;



//variables para animación
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
Model botes;
Model tamales;

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

///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;


//NEW// Keyframes
float posXtamales = 100.0, posYtamales = 0.0, posZtamales = -350.0;
float	movtamales_x = 0.0f, movtamales_z = 0.0f;
float giroTamales = 0;

#define MAX_FRAMES 30
int i_max_steps = 90;
int i_curr_steps = 14;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movTamales_x;		//Variable para PosicionX
	float movTamales_z;		//Variable para PosicionY
	float movTamales_xInc;		//Variable para IncrementoX
	float movTamales_zInc;		//Variable para IncrementoY
	float giroTamales;
	float giroTamalesInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 14;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movTamales_x = movtamales_x;
	KeyFrame[FrameIndex].movTamales_z = movtamales_z;
	KeyFrame[FrameIndex].giroTamales = giroTamales;

	FrameIndex++;
}

void resetElements(void)
{

	movtamales_x = KeyFrame[0].movTamales_x;
	movtamales_z = KeyFrame[0].movTamales_z;
	giroTamales = KeyFrame[0].giroTamales;
}

void interpolation(void)
{
	KeyFrame[playIndex].movTamales_xInc = (KeyFrame[playIndex + 1].movTamales_x - KeyFrame[playIndex].movTamales_x) / i_max_steps;
	KeyFrame[playIndex].movTamales_zInc = (KeyFrame[playIndex + 1].movTamales_z - KeyFrame[playIndex].movTamales_z) / i_max_steps;
	KeyFrame[playIndex].giroTamalesInc = (KeyFrame[playIndex + 1].giroTamales - KeyFrame[playIndex].giroTamales) / i_max_steps;

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
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movtamales_x += KeyFrame[playIndex].movTamales_xInc;
			movtamales_z += KeyFrame[playIndex].movTamales_zInc;
			giroTamales += KeyFrame[playIndex].giroTamalesInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/

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

	lampara = Model();
	lampara.LoadModel("Models/lampara/lampara.obj");

	cartel = Model();
	cartel.LoadModel("Models/cartel/Anuncion_neon.obj");

	botes = Model();
	botes.LoadModel("Models/bote_Basura/trashcan1.obj");

	tamales = Model();
	tamales.LoadModel("Models/carrito/carrito.obj");
	//Tux_M tux = Tux_M(
	//	glm::vec3(0.0f),
	//	"Models/tux/body.obj", // body
	//	"Models/tux/larm.obj",
	//	"Models/tux/rarm.obj",
	//	"Models/tux/lfoot.obj",
	//	"Models/tux/rfoot.obj"
	//);
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


	//luz direccional, sólo 1 y siempre debe de existir
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

	//PUESTO DE TORTAS
	spotLights[0] = SpotLight(1.0f, 0.5f, 0.0f,//color de la luz
		50.0f, 1.0f, //En esta si es importante el segundo valor porque nos da la intesidad de la luz  el primero no importa
		-120.0f, 40.0f, 150.0f,//Poscion, 
		10.0f, -1.0f, 0.0f,//Valores de direccion 
		0.5f, 0.7f, 0.1f,//valores de la ecuacion de segundo grado, no puede dar 0 este es que tanto se agranda o hace peque�a la luz
		200.0f);//Apertura de cono
	spotLightCount++;

	//PUESTO DE HOTDOGS
	spotLights[1] = SpotLight(1.0f, 0.5f, 0.0f,//color de la luz
		50.0f, 1.0f, //En esta si es importante el segundo valor porque nos da la intesidad de la luz  el primero no importa
		120.0f, 40.0f, 50.0f,//Poscion, 
		10.0f, -1.0f, 0.0f,//Valores de direccion 
		0.5f, 0.7f, 0.1f,//valores de la ecuacion de segundo grado, no puede dar 0 este es que tanto se agranda o hace peque�a la luz
		200.0f);//Apertura de cono
	spotLightCount++;

	//PUESTO ITALIANO
	spotLights[2] = SpotLight(1.0f, 0.5f, 0.0f,//color de la luz
		50.0f, 1.0f, //En esta si es importante el segundo valor porque nos da la intesidad de la luz  el primero no importa
		120.0f, 40.0f, -80.0f,//Poscion, 
		10.0f, -1.0f, 0.0f,//Valores de direccion 
		0.5f, 0.7f, 0.1f,//valores de la ecuacion de segundo grado, no puede dar 0 este es que tanto se agranda o hace peque�a la luz
		200.0f);//Apertura de cono
	spotLightCount++;

	//PUESTO ARABE
	spotLights[3] = SpotLight(1.0f, 0.5f, 0.0f,//color de la luz
		50.0f, 1.0f, //En esta si es importante el segundo valor porque nos da la intesidad de la luz  el primero no importa
		120.0f, 30.0f, -240.0f,//Poscion, 
		10.0f, -1.0f, 0.0f,//Valores de direccion 
		0.5f, 0.7f, 0.1f,//valores de la ecuacion de segundo grado, no puede dar 0 este es que tanto se agranda o hace peque�a la luz
		200.0f);//Apertura de cono
	spotLightCount++;


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

	start_time = std::chrono::high_resolution_clock::now();
	//******************************************FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF******************************************/
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);  //VARIABLE MAT PARA DECLARA DATOS DE X,Y,Z INICILES PARA EL AVION 
	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movTamales_x = -10.0f;
	KeyFrame[0].movTamales_z = 0.0f;
	KeyFrame[0].giroTamales = 90;

	KeyFrame[1].movTamales_x = -10.0f;
	KeyFrame[1].movTamales_z = 200.0f;
	KeyFrame[1].giroTamales = 90;

	KeyFrame[2].movTamales_x = -10.0f;
	KeyFrame[2].movTamales_z = 400.0f;
	KeyFrame[2].giroTamales = 90;

	KeyFrame[3].movTamales_x = -50.0f;
	KeyFrame[3].movTamales_z = 400.0f;
	KeyFrame[3].giroTamales = 0;

	KeyFrame[4].movTamales_x = -100.0f;
	KeyFrame[4].movTamales_z = 400.0f;
	KeyFrame[4].giroTamales = 0;

	KeyFrame[5].movTamales_x = -175.0f;
	KeyFrame[5].movTamales_z = 400.0f;
	KeyFrame[5].giroTamales = 0;

	KeyFrame[6].movTamales_x = -175.0f;
	KeyFrame[6].movTamales_z = 400.0f;
	KeyFrame[6].giroTamales = -90;

	KeyFrame[7].movTamales_x = -175.0f;
	KeyFrame[7].movTamales_z = 200.0f;
	KeyFrame[7].giroTamales = -90;

	KeyFrame[8].movTamales_x = -175.0f;
	KeyFrame[8].movTamales_z = -25.0f;
	KeyFrame[8].giroTamales = -90;

	KeyFrame[9].movTamales_x = -175.0f;
	KeyFrame[9].movTamales_z = -10.0f;
	KeyFrame[9].giroTamales = -180;
	//
	KeyFrame[10].movTamales_x = 20.0f;
	KeyFrame[10].movTamales_z = -1.0f;
	KeyFrame[10].giroTamales = -180;

	KeyFrame[11].movTamales_x = 20.0f;
	KeyFrame[11].movTamales_z = -1.0f;
	KeyFrame[11].giroTamales = -180;



	
	//******************************************FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF*****************
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		
		//PARA EL ENCENDIDO / APAGADO DE LUCES TIPO SPOTLIGTH
		//spotLights[0].SetIntensity(mainWindow.EnciendeApagaLuces());//Agregando el valor de la intensidad
		spotLights[0].SetIntensity(mainWindow.EnciendeApagaLucesdifusa());
		spotLights[0].SetAmbientIntensity(mainWindow.EnciendeApagaLucesAmbiente());

		spotLights[1].SetIntensity(mainWindow.EnciendeApagaLucesdifusa());
		spotLights[1].SetAmbientIntensity(mainWindow.EnciendeApagaLucesAmbiente());

		spotLights[2].SetIntensity(mainWindow.EnciendeApagaLucesdifusa());
		spotLights[2].SetAmbientIntensity(mainWindow.EnciendeApagaLucesAmbiente());

		spotLights[3].SetIntensity(mainWindow.EnciendeApagaLucesdifusa());
		spotLights[3].SetAmbientIntensity(mainWindow.EnciendeApagaLucesAmbiente());


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
			// Aero Mode
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
		model = glm::translate(model, glm::vec3(5.0f, -5.0f, 220.0f));
		model = glm::scale(model, glm::vec3(7.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Stage_M.RenderModel();

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
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -370.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -370.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -370.0f));
		model = glm::scale(model, glm::vec3(10.0f, 12.0f, 10.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 10.0f, -400.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		botes.RenderModel();

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXtamales + movtamales_x, posYtamales , posZtamales + movtamales_z); //POS INICIAL+VALOR DE LA ANIMACION
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(6.0f, 10.0f, 8.0f));
		model = glm::rotate(model, giroTamales * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tamales.RenderModel();


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
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetu += 0.001;
		toffsetv += 0.0;
		//para que no se desborde la variable
		if (toffsetu > 1.0)
			toffsetu = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);

		//pasar a la variable uniform el valor actualizado
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
		//model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
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
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
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
			printf("movAvion_x es: %f\n", movtamales_x);
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
			movtamales_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movtamales_x);
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
