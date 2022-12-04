#pragma once
#include <string>
#include <glm.hpp>
#include"Model.h"

/*
Treat the Tux Penguin model as an Object

Tux has the following abilities:

- Move in the XY Axis
- when walking move its arms and legs
*/

const float STEP_INC = 1.0f;


class Tux_M
{
	private:
		glm::vec3 pos;
		glm::vec3 dir;

		Model body_M, larm_M, rarm_M, lfoot_M, rfoot_M;

		// limit these to certain values
		float left_arm_ang;  
		float right_arm_ang;
		float left_foot_ang;
		float right_foot_ang;

		bool larm_d, rarm_d;

	public:
		Tux_M(std::string body_model,
			std::string left_arm_model,
			std::string right_arm_model,
			std::string left_foot_model,
			std::string right_foot_model);
		void walkAnimation();
		void draw(GLuint uniformModel);
		void move(glm::vec3 &newPos, glm::vec3 &newDir, GLuint &uniformModel);
			/*
				Recibir un vector de pocisión y otro de sentido, el cua está ligado a la
				camara, esto para hacer las traslaciones correspondientes.

				Para cada movimiento que se realice se debe redibujar el modelo.
			*/

			// trasladar a newPos
			// incrementar animación de caminar
			// 
};

