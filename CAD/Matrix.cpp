#include "Matrix.h"

glm::mat4 Matrix::PerspectiveProjection(float fov, float aspect, float n, float f)
{
	float ctg = 1 / glm::tan(glm::radians(fov / 2.f));
	return glm::mat4(
		ctg / aspect, 0, 0, 0,
		0, ctg, 0, 0,
		0, 0, -(f + n) / (f - n), -1.f,
		0, 0, (-2 * f * n) / (f - n), 0
	);
}

glm::mat4 Matrix::Scale(float ratio)
{
	return glm::mat4(
		ratio, 0.f, 0.f, 0.f,
		0.f, ratio, 0.f, 0.f,
		0.f, 0.f, ratio, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

glm::mat4 Matrix::LookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	const glm::vec3 newZ = glm::normalize(eye - center);
	const glm::vec3 newX = glm::normalize(glm::cross(up, newZ));	//
	const glm::vec3 newY = glm::cross(newZ, newX);

	return glm::mat4(
		newX.x, newY.x, newZ.x, 0.f,
		newX.y, newY.y, newZ.y, 0.f,
		newX.z, newY.z, newZ.z, 0.f,
		-glm::dot(newX, eye), -glm::dot(newY, eye), -glm::dot(newZ, eye), 1.f
	);
}

glm::mat4 Matrix::RotationY(float angle)
{
	return glm::mat4(
		glm::cos(angle), 0.f, -glm::sin(angle), 0.f,
		0.f, 1.f, 0.f, 0.f,
		glm::sin(angle), 0.f, glm::cos(angle), 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

glm::mat4 Matrix::RotationZ(float angle)
{
	return glm::mat4(
		1.0f, 0.f, 0.f, 0.f,
		0.f, glm::cos(angle), glm::sin(angle), 0.f,
		0.f, -glm::sin(angle), glm::cos(angle), 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}
