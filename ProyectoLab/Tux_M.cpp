#include "Tux_M.h"
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

const float arm_angle_limit = 45.0f;
const float foot_angle_limit = 45.0f;

Tux_M::Tux_M(
	std::string body_model,
	std::string left_arm_model,
	std::string right_arm_model,
	std::string left_foot_model,
	std::string right_foot_model
) {
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

void Tux_M::move(glm::vec3 &newPos, glm::vec3 &newDir, GLuint &uniformModel){
/*
	Recibir un vector de pocisión y otro de sentido, el cua está ligado a la
	camara, esto para hacer las traslaciones correspondientes.

	Para cada movimiento que se realice se debe redibujar el modelo.
*/

// trasladar a newPos
// incrementar animación de caminar

	glm::mat4 model = glm::mat4(1.0);
	pos = glm::vec3(newPos.x + 10.0f, 0, newPos.z);
	dir = newDir;

	//Transform all parts
	printf("moving to %f, %f, %f\n", pos.x, pos.y, pos.z);
	model = glm::translate(model, pos);
	/*
	Rotar de acuerdo al angulo que exista entre el vector de dirección actual
	y el nuevo.
	*/
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	body_M.RenderModel();
	larm_M.RenderModel();
	rarm_M.RenderModel();
	lfoot_M.RenderModel();
	rfoot_M.RenderModel();

}