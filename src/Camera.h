#pragma once
#include <glm/glm.hpp>

class Camera
{
private:
	glm::mat4 currentView;

public:
	Camera() :
		Camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {}
	Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

	glm::mat4 GetViewMatrix() const { return this->currentView; }
	void RotateX(float angleInRad);
	void RotateY(float angleInRad);
	void RotateZ(float angleInRad);
	void Scale(float ratio);
	void Translate(glm::vec3 vector);
};
