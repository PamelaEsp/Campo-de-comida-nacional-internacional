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

		// ### MOVEMENT ###
		glm::vec3 pos; // Donde está
		glm::vec3 front; // hacia donde apunta
		glm::vec3 worldUp, right, up;
		GLfloat yaw;
		GLfloat pitch;
		GLfloat moveSpeed;
		GLfloat turnSpeed;
		GLfloat rotation_angle;


		Model body_M, larm_M, rarm_M, lfoot_M, rfoot_M;

		// limit these to certain values
		float left_arm_ang;  
		float right_arm_ang;
		float left_foot_ang;
		float right_foot_ang;

		bool larm_d, rfoot_d;
		void update();

	public:
		Tux_M(
			glm::vec3 startingPosition,
			std::string body_model,
			std::string left_arm_model,
			std::string right_arm_model,
			std::string left_foot_model,
			std::string right_foot_model
		);

		void keyControl(bool* keys, GLfloat deltaTime);
		void mouseControl(GLfloat xChange, GLfloat yChange);

		glm::vec3 getPos() { return pos; };
		glm::vec3 getDir() { return front; };
		glm::vec3 getWorldUp() { return worldUp; };

		void walkAnimation();
		void draw(GLuint uniformModel);
		void move(GLuint &uniformModel);
};

