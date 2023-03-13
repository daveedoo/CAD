#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	this->currentView = glm::mat4(1.f);
	//this->currentView = glm::lookAt(eye, center, up);
}

void Camera::RotateX(float a)
{
	//this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(1, 0, 0));
	this->currentView *= glm::mat4(
		1.f,	0.f,				0.f,				0.f,
		0.f,	glm::cos(a),	glm::sin(a),	0.f,
		0.f,	-glm::sin(a),	glm::cos(a),	0.f,
		0.f,	0.f,				0.f,				1.f
	);
}

void Camera::RotateY(float a)
{
	//this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(0, 1, 0));
	this->currentView *= glm::mat4(
		glm::cos(a),	0.f,				-glm::sin(a),	0.f,
		0.f,				1.f,				0.f,				0.f,
		glm::sin(a),	0.f,				glm::cos(a),	0.f,
		0.f,				0.f,				0.f,				1.f
	);
}

//void Camera::RotateZ(float angleInRad)
//{
//	this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(0, 0, 1));
//}

void Camera::Scale(float ratio)
{
	//this->currentView = glm::scale(this->currentView, glm::vec3(ratio));
	// it is written like its transposed
	this->currentView *= glm::mat4(
		ratio, 0.f, 0.f, 0.f,
		0.f, ratio, 0.f, 0.f,
		0.f, 0.f, ratio, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void Camera::Translate(glm::vec3 v)
{
	this->currentView *= glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		v.x, v.y, v.z, 1.f
	);
}
