#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;  // Camera posision
	position = glm::vec3(0.0f, 2.0f, -5.0f); // Viendo al tux desde atras
	tuxPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	worldUp = startUp;
	yaw = startYaw;		// angulo vista hacia izquierda/derecha 
	pitch = startPitch; // angulo vista hacia arriba/abajo

	// TODO: Instead of using a calculated front, always focus on TUX
	front = glm::vec3(0.0f, 0.0f, 1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	// Limit the user from going under the plane
	if (position.y <= 0.0f)
	{
		position.y = 0.1f;
	}

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	// TODO: Tomar en cuenta la pocisión del tux
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	
	//front.y = 0.0f; // Restrict the user from lookinng up and back
	
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

// ### 3rd Person Mode ###
void Camera::adjustCamera(glm::vec3 tuxPos, glm::vec3 tuxDir, glm::vec3 tuxWorldUp) {
	/*Calculamos la posición y sentido de la camara a partir de la pocisión del objeto*/

	float scale = 4.0f;

	position = tuxPos - scale * tuxDir;
	// NOTE: Utilizamos tuxDir por que es la misma dirección a la que debe apuntar la cámara
	front = tuxDir;
	right = glm::normalize(glm::cross(front, tuxWorldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
