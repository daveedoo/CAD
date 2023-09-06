#pragma once
#include <glm/glm.hpp>

struct Transformation
{
	glm::mat4x4 worldMatrix;

	Transformation(glm::mat4 matrix = glm::mat4(1.f))
		: worldMatrix(matrix) {}
};
