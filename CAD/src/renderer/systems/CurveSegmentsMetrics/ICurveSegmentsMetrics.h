#pragma once
#include <array>
#include <glm/glm.hpp>

class ICurveSegmentsMetrics
{
public:
	virtual unsigned int CalculateSegmentsCount(std::array<glm::vec3, 4> curveCoefficients) = 0;
};
