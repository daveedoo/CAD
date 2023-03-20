#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../Matrix.h"



Camera::Camera(float fov, float aspect, float near, float far)
{
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
	eye *= 1/this->scale;
	glm::vec3 up(0.f, 1.f, 0.f);

	this->view = Matrix::LookAt(eye + translation, this->translation, up);
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
	glm::vec3 vectorFixed = glm::vec3(Matrix::RotationY(-glm::radians(this->yaw - YAW_ZERO)) * glm::vec4(v, 1.f));

	this->translation += vectorFixed;
	UpdateViewMatrix();
}

void Camera::SetPerspectiveProjection(float fov, float aspect, float near, float far)
{
	this->projection = Matrix::PerspectiveProjection(fov, aspect, near, far);
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