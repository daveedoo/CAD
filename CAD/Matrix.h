#pragma once
#include <glm/glm.hpp>

static class Matrix
{
public:
	static glm::mat4 PerspectiveProjection(float fov, float aspect, float near, float far);
	static glm::mat4 Scale(float ratio);
	static glm::mat4 LookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

	static glm::mat4 RotationY(float angle);
	static glm::mat4 RotationZ(float angle);
};

