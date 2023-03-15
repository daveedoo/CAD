#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Camera::Camera()
{
	//this->yaw = 0.f;
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	const float P = glm::radians(this->pitch);
	const float Y = glm::radians(this->yaw);

	//const glm::mat4 rotZ = glm::mat4(
	//	1.0f, 0.f, 0.f, 0.f,
	//	0.f, glm::cos(P), glm::sin(P), 0.f,
	//	0.f, -glm::sin(P), glm::cos(P), 0.f,
	//	0.f, 0.f, 0.f, 1.f
	//);
	//const glm::mat4 rotY = glm::mat4(
	//	glm::cos(yaw), 0.f, -glm::sin(yaw), 0.f,
	//	0.f, 1.f, 0.f, 0.f,
	//	glm::sin(yaw), 0.f, glm::cos(yaw), 0.f,
	//	0.f, 0.f, 0.f, 1.f
	//);
	//const glm::mat4 scaleMtx = glm::mat4(
	//	this->scale, 0.f, 0.f, 0.f,
	//	0.f, this->scale, 0.f, 0.f,
	//	0.f, 0.f, this->scale, 0.f,
	//	0.f, 0.f, 0.f, 1.f
	//);
	//this->view = rotY * rotZ * scaleMtx;


	glm::vec3 eye(
		glm::cos(P) * glm::cos(Y),
		glm::sin(P),
		glm::cos(P) * glm::sin(Y)
	);
	glm::vec3 center(0.f, 0.f, 0.f);
	glm::vec3 up(0.f, 1.f, 0.f);

	this->view = glm::scale(glm::lookAt(eye, center, up), glm::vec3(this->scale));	// TODO: scaling here?
}


void Camera::RotatePitch(float angle)
{
	this->pitch -= angle;
	this->pitch = glm::clamp(this->pitch, -90.f, 90.f);
	UpdateViewMatrix();
}

void Camera::RotateYaw(float angle)
{
	this->yaw += angle;
	this->yaw = fmod(this->yaw, 360.f);
	UpdateViewMatrix();
}

void Camera::Scale(float ratio)
{
	this->scale *= ratio;
	UpdateViewMatrix();
}

void Camera::Translate(glm::vec3 v)
{
	//this->currentView *= glm::mat4(
	//	1.f, 0.f, 0.f, 0.f,
	//	0.f, 1.f, 0.f, 0.f,
	//	0.f, 0.f, 1.f, 0.f,
	//	v.x, v.y, v.z, 1.f
	//);
}
