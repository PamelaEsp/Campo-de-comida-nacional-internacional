#include "Tux_M.h"
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

const float arm_angle_limit = 24.0f;
const float foot_angle_limit = 20.0f;
const float TUX_SCALE = 2.0f;


/*
TODO:

Tux debe manejar su pocisión en XYZ en el plano en todo momento, ya que a partir del tux
se íra moviendo la camara haciendo uso de LookAt(), el cual nos permitira siempre tener 
al tux en el foco de la camara sin importar a donde nos movemos.

*/

Tux_M::Tux_M(
	glm::vec3 pos,
	std::string body_model,
	std::string left_arm_model,
	std::string right_arm_model,
	std::string left_foot_model,
	std::string right_foot_model
) {
	// ### Pocision y direccion tux ###
	pos = glm::vec3(0.0f);
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 90.0f;  // hacia al frente
	pitch = 0.0f; 
	CAMERA_3D_MODE_SCALE = 70.0f;

	//moveSpeed = .f;
	turnSpeed = 0.5f;

	// Limit arms and foot movement
	larm_d = true; // front
	rfoot_d = true; // back
	left_arm_ang = 0.0f;
	right_arm_ang = 0.0f;
	right_foot_ang = 0.0f;
	left_foot_ang = 0.0f;

	update();

	// ### Carga de modelos ### 
	body_M = Model();
	larm_M = Model();
	rarm_M = Model();
	lfoot_M = Model();
	rfoot_M = Model();

	rotation_angle = 0.0f;

	body_M.LoadModel(body_model);
	larm_M.LoadModel(left_arm_model);
	rarm_M.LoadModel(right_arm_model);
	rfoot_M.LoadModel(right_foot_model);
	lfoot_M.LoadModel(left_foot_model);
}

void Tux_M::walkAnimation() {
	float step = 0.2f;

	// Brazo izquierdo
	if (larm_d){ // front
		if (left_arm_ang < arm_angle_limit) {
			// it is in the acceptable range for movement
			left_arm_ang+= step;
		}
		else if (left_arm_ang >= arm_angle_limit) {
			// Change direction
			larm_d = false;
			left_arm_ang-= step;
		}
	}
	else { // back
		if (left_arm_ang > -arm_angle_limit) {
			// it is in the acceptable range for movement
			left_arm_ang-= step;
		}
		else if (left_arm_ang <= arm_angle_limit) {
			// Change direction
			larm_d = true;
			left_arm_ang+= step;
		}
	}

	right_arm_ang = left_arm_ang;

	// Pie Derecho
	if (rfoot_d) { // front
		if (left_foot_ang < foot_angle_limit) {
			// it is in the acceptable range for movement
			left_foot_ang += step;
		}
		else if (left_foot_ang >= foot_angle_limit) {
			// Change direction
			rfoot_d = false;
			left_foot_ang -= step;
		}
	}
	else { // back
		if (left_foot_ang > -foot_angle_limit) {
			// it is in the acceptable range for movement
			left_foot_ang -= step;
		}
		else if (left_foot_ang <= foot_angle_limit) {
			// Change direction
			rfoot_d = true;
			left_foot_ang += step;
		}
	}

}

void Tux_M::keyControl(bool* keys, GLfloat deltaTime)
{
	// Calcula la posición del objeto a partir del vector dirección y hacia donde queremos ir
	
	GLfloat velocity = STEP_INC * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		pos += front * velocity;
		walkAnimation();
	}

	if (keys[GLFW_KEY_S])
	{
		pos -= front * velocity;
		walkAnimation();
	}

	if (keys[GLFW_KEY_A])
	{
		pos -= right * velocity;
		walkAnimation();
	}

	if (keys[GLFW_KEY_D])
	{
		pos += right * velocity;
		walkAnimation();
	}
}

void Tux_M::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	/*Make rotation angle the actual yaw minus the old yaw*/

	rotation_angle = yaw;
	yaw += xChange;  // lados

	rotation_angle = yaw - rotation_angle;

	update();
}


void Tux_M::update()
{

	// TODO: As pitch is always constant, reuse it.
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));

	front.y = 0.0f; // No changes in y

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


void Tux_M::move(GLuint &uniformModel){

	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 aux;
	
	model = glm::translate(model, glm::vec3(pos.x, -2.0f, pos.z));
	model = glm::scale(model, glm::vec3(TUX_SCALE));
	//model = glm::rotate(model, glm::radians(yaw), glm::vec3(pos.x, 1.0f,pos.z));
	//model *= glm::transpose(glm::lookAt(pos, pos + front, up));
	
	// TODO: Fix Tux to alway look at front, Rotate using Quaternions? 
	
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	body_M.RenderModel();

	aux = model;
	model = glm::rotate(model, glm::radians(left_arm_ang), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	larm_M.RenderModel();
	
	// FIXME: if rarm_ang is -larm_ang, then why does it exist?
	model = aux;
	model = glm::rotate(model, glm::radians(right_arm_ang), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	rarm_M.RenderModel();

	model = aux;
	model = glm::rotate(model, glm::radians(right_arm_ang), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	lfoot_M.RenderModel();
	model = aux;
	model = glm::rotate(model, glm::radians(-right_arm_ang), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	rfoot_M.RenderModel();

}