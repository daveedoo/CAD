#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "maths\Matrix.h"



Camera::Camera(float fov, int viewportWidth, int viewportHeight, float near, float far)
{
	UpdateViewMatrix();
	UpdatePerspective();
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

void Camera::SetPerspectiveProjection(float fov, int viewportWidth, int viewportHeight, float near, float far)
{
	this->projection = Matrix::PerspectiveProjection(fov, static_cast<float>(viewportWidth) / viewportHeight, near, far);
}

void Camera::SetFov(float fov)
{
	this->fov = fov;
	UpdatePerspective();
}

void Camera::SetViewportSize(int viewportWidth, int viewportHeight)
{
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	UpdatePerspective();
}

void Camera::GetViewportSize(int& viewportWidth, int& viewportHeight) const
{
	viewportWidth = this->viewportWidth;
	viewportHeight = this->viewportHeight;
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
	SetPerspectiveProjection(this->fov, this->viewportWidth, this->viewportHeight, this->near, this->far);
}
