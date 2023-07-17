#pragma once
#include <glm/glm.hpp>

struct ScreenPosition
{
	glm::vec3 position;

	ScreenPosition(const glm::dvec3& position = glm::vec3(0.f))
		: position(position)
	{
	}
};
