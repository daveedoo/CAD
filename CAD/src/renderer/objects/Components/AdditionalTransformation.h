#pragma once
#include <glm/glm.hpp>
#include "ScaleRotation.h"

struct AdditionalTransformation
{
	glm::vec3 centerPoint;
	ScaleRotation scaleRotation;

	AdditionalTransformation(glm::vec3 centerPoint, ScaleRotation sr = ScaleRotation())
		: centerPoint(centerPoint), scaleRotation(sr) {}
};
