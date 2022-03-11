#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	this->currentView = glm::lookAt(eye, center, up);
}

void Camera::RotateX(float angleInRad)
{
	this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(1, 0, 0));
}

void Camera::RotateY(float angleInRad)
{
	this->currentView = glm::rotate(this->currentView, angleInRad, glm::vec3(0, 1, 0));
}
