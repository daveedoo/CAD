#pragma once
#include <glm\glm.hpp>

static class Bernstein
{
public:
	static glm::vec3 CalculateBernsteinValue(const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3, float t);
};

