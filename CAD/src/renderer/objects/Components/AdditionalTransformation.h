#pragma once
#include <glm/glm.hpp>
#include "Rotation.h"

struct AdditionalTransformation
{
	glm::vec3 centerPoint;
	float scale;
	Rotation rotation;

	AdditionalTransformation(glm::vec3 centerPoint, float scale, Rotation rotation = Rotation())
		: centerPoint(centerPoint), scale(scale), rotation(rotation) {}
};
