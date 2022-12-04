#include "Tux_M.h"
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

const float arm_angle_limit = 45.0f;
const float foot_angle_limit = 45.0f;

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

	moveSpeed = 0.5f;
	turnSpeed = 0.5f;

	update();

	// ### Carga de modelos ### 
	body_M = Model();
	larm_M = Model();
	rarm_M = Model();
	lfoot_M = Model();
	rfoot_M = Model();
	
	// Limit arms and foot movement
	left_arm_ang = -45.0f;
	right_arm_ang = -left_arm_ang;
	larm_d = true; // front
	rarm_d = false; // back

	right_foot_ang = -20.0f;
	left_foot_ang = -right_foot_ang;

	body_M.LoadModel(body_model);
	larm_M.LoadModel(left_arm_model);
	rarm_M.LoadModel(right_arm_model);
	rfoot_M.LoadModel(right_foot_model);
	lfoot_M.LoadModel(left_foot_model);
}

void Tux_M::walkAnimation() {

	// Brazo izquierdo
	if (larm_d){ // front
		if (left_arm_ang < arm_angle_limit) {
			// it is in the acceptable range for movement
			left_arm_ang++;
		}
		else if (left_arm_ang >= arm_angle_limit) {
			// Change direction
			larm_d = false;
			left_arm_ang--;
		}
	}
	else { // back
		if (left_arm_ang > arm_angle_limit) {
			// it is in the acceptable range for movement
			left_arm_ang--;
		}
		else if (left_arm_ang <= arm_angle_limit) {
			// Change direction
			larm_d = true;
			left_arm_ang++;
		}
	}

}
void Tux_M::draw(GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0);
	
	//Transform all parts
	model = glm::translate(model, pos);
	/*
	Rotar de acuerdo al angulo que exista entre el vector de dirección actual
	y el nuevo.
	*/
	// model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	

	body_M.RenderModel();
}

void Tux_M::keyControl(bool* keys, GLfloat deltaTime)
{
	// Calcula la posición del objeto a partir del vector dirección y hacia donde queremos ir
	
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		pos += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		pos -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		pos -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		pos += right * velocity;
	}
}

void Tux_M::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;  // lados

	// No permitimos movimientos de arriba abajo, por lo que pitch nunca se modifica
	//pitch += yChange; // arriba abajo

	//if (pitch > 89.0f)
	//{
	//	pitch = 89.0f;
	//}

	//if (pitch < -89.0f)
	//{
	//	pitch = -89.0f;
	//}

	update();
}

void Tux_M::update()
{
	// NOTE: yaw is always constant
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//front.y = sin(glm::radians(pitch));
	front.y = 0.0f; // Restrict the user from lookinng up and back

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


void Tux_M::move(glm::vec3 &newPos, glm::vec3 &newDir, GLuint &uniformModel){

	glm::mat4 model = glm::mat4(1.0);

	model = glm::translate(model, glm::vec3(pos.x, -2.0f, pos.z));
	/*
	TODO: Rotar de acuerdo al angulo que exista entre el vector de dirección actual
	y el nuevo.
	*/

	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	body_M.RenderModel();
	larm_M.RenderModel();
	rarm_M.RenderModel();
	lfoot_M.RenderModel();
	rfoot_M.RenderModel();

}