#pragma once
#include <glm/glm.hpp>

struct ScaleRotation
{
	glm::vec3 scale;
	float axisFi;
	float axisLambda;
	float angle;

	ScaleRotation(glm::vec3 scale = glm::vec3(1.f), float axisFi = 0.f, float axisLambda = 0.f, float angle = 0.f)
		: scale(scale), axisFi(axisFi), axisLambda(axisLambda), angle(angle) {}
};
