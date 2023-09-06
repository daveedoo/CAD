#pragma once
#include <glm/glm.hpp>

struct Position
{
	glm::vec3 position;

	Position(glm::vec3 val = glm::vec3(0.f))
		: position(val) {}
};
