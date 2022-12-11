#include "Camera.h"

/* This class handles the :
* 3rd cam mode
* isometric cam mode
* 
* the 3rd cam mode is handled betweeen tux and this object.
*/

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;				 // Camera posision
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

void Camera::keyControl(bool *keys, GLfloat deltaTime)
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

	// front.y = 0.0f; // Restrict the user from lookinng up and back

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

/* ### 3rd Person Mode ###
*	Adjust the camera to always be positioned behind TUX and looking at front.
* 
*  Expects to receive the tux pos, tux direction and the up vector of tux.
*/
void Camera::adjustCamera(glm::vec3 tuxPos, glm::vec3 tuxDir, glm::vec3 tuxWorldUp, float camera_scale)
{
	// Utilizamos tuxDir por que es la misma direcci�n a la que debe apuntar la c�mara
	position = tuxPos - camera_scale * tuxDir;
	position.y = 30.0f;

	front = tuxDir;
	right = glm::normalize(glm::cross(front, tuxWorldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}
