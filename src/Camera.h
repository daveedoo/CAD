#pragma once
#include <glm/glm.hpp>

class Camera
{
private:
	glm::mat4 currentView;

public:
	Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

	glm::mat4 GetViewMatrix() const { return this->currentView; }
	void RotateX(float angleInRad);
	void RotateY(float angleInRad);
};
