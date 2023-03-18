#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../Matrix.h"



Camera::Camera(float fov, float aspect, float near, float far)
{
	//this->yaw = 0.f;
	UpdateViewMatrix();
	SetPerspectiveProjection(fov, aspect, near, far);
}

void Camera::UpdateViewMatrix()
{
	const float P = glm::radians(this->pitch);
	const float Y = glm::radians(this->yaw);

	glm::vec3 eye(
		glm::cos(P) * glm::cos(Y),
		glm::sin(P),
		glm::cos(P) * glm::sin(Y)
	);
	//glm::vec3 center(0.f, 0.f, 0.f);
	glm::vec3 up(0.f, 1.f, 0.f);

	//this->view = glm::scale(glm::lookAt(eye + translation, this->translation, up), glm::vec3(this->scale));	// TODO: scaling here?
	//this->view = glm::lookAt(eye + translation, this->translation, up) * Matrix::Scale(this->scale);
	this->view = Matrix::LookAt(eye + translation, this->translation, up) * Matrix::Scale(this->scale);
}


void Camera::RotatePitch(float angle)
{
	this->pitch -= angle;
	this->pitch = glm::clamp(this->pitch, -89.f, 89.f);
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
	//glm::vec4 newT = this->view * glm::vec4(v, 0.f);
	//this->translation += glm::vec3(newT.x, newT.y, newT.z);
	//glm::mat4 id = glm::identity();

	//glm::vec4 newT = glm::rotate(glm::mat4(1.f), glm::radians(this->yaw), glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(v, 0.f);
	//this->translation += glm::vec3(newT.x, newT.y, newT.z);
	//UpdateViewMatrix();

	this->translation += v;
	UpdateViewMatrix();
	
	//this->currentView *= glm::mat4(
	//	1.f, 0.f, 0.f, 0.f,
	//	0.f, 1.f, 0.f, 0.f,
	//	0.f, 0.f, 1.f, 0.f,
	//	v.x, v.y, v.z, 1.f
	//);
}

void Camera::SetPerspectiveProjection(float fov, float aspect, float near, float far)
{
	this->projection = Matrix::PerspectiveProjection(fov, aspect, near, far);
	//this->projection = glm::perspective(glm::radians(fov), aspect, n, f);
}

void Camera::SetFov(float fov)
{
	this->fov = fov;
	UpdatePerspective();
}

void Camera::SetAspect(float aspect)
{
	this->aspect = aspect;
	UpdatePerspective();
}

void Camera::SetNear(float near)
{
	this->near = near;
	UpdatePerspective();
}

void Camera::SetFar(float far)
{
	this->far = far;
	UpdatePerspective();
}

void Camera::UpdatePerspective()
{
	SetPerspectiveProjection(fov, aspect, near, far);
}