#pragma once
#include <glm/glm.hpp>

class Camera
{
private:
	glm::mat4 view;

	//float roll = 0.f;
	float pitch = 0.f;
	float yaw = 270.f;	// defualt camera position is (0, 0, -1)
	float scale = 1.0;

	void UpdateViewMatrix();

public:
	Camera();

	glm::mat4 GetViewMatrix() const { return this->view; }

	/// Rotations around Y axis in degrees
	void RotateYaw(float angle);
	/// Rotations between the poles in degrees
	void RotatePitch(float angle);
	void Scale(float ratio);
	void Translate(glm::vec3 vector);
};
