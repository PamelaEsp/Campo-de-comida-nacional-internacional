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
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }

	//Para apagado encendido de luces
	GLfloat EnciendeApagaLucesdifusa() { return enciendeLucesDifusa; }
	GLfloat EnciendeApagaLucesAmbiente() { return enciendeLucesAmbiente; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	bool isDay();
	void toggleDay();

	bool pointLightsButtonPress;
	bool getPointLightsButtonsPress() { return pointLightsButtonPress; };

	bool getCameraMode() { return camera_mode; };
	void setCameraMode(bool mode);

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
	GLfloat muevex;

	//Para encendido de luces por teclado 
	GLfloat enciendeLucesDifusa;
	GLfloat enciendeLucesAmbiente;

	bool day_state;
	bool camera_mode; // true: 3rd person, false: isometric mode

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

