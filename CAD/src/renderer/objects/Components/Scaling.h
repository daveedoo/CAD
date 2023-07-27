#pragma once
#include <glm/glm.hpp>

struct Scaling
{
	glm::vec3 scale;

	Scaling(glm::vec3 scale = glm::vec3(1.f))
		: scale(scale) {}
	operator glm::vec3() const { return this->scale; }
};