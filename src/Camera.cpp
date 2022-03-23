#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	this->currentView = glm::mat4(1.f);
}

void Camera::RotateX(float angleInRad)
{
	this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(1, 0, 0));
}

void Camera::RotateY(float angleInRad)
{
	this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(0, 1, 0));
}

void Camera::RotateZ(float angleInRad)
{
	this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(0, 0, 1));
}

void Camera::Scale(float ratio)
{
	this->currentView = glm::scale(this->currentView, glm::vec3(ratio));
}

void Camera::Translate(glm::vec3 vector)
{
	this->currentView = glm::translate(this->currentView, vector);
}
