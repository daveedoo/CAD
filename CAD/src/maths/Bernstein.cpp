#include "Bernstein.h"
#include <stdexcept>

glm::vec3 Bernstein::CalculateBernsteinValue(const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3, float t)
{
	// De Casteljau
	if (t < 0 || t > 1)
		throw std::invalid_argument("Bernstein::CalculateBernsteinValue - invalid 't' arg");

	glm::vec3 b1_0 = b1 * t + b0 * (1 - t);
	glm::vec3 b1_1 = b2 * t + b1 * (1 - t);
	glm::vec3 b1_2 = b3 * t + b2 * (1 - t);

	glm::vec3 b2_0 = b1_1 * t + b1_0 * (1 - t);
	glm::vec3 b2_1 = b1_2 * t + b1_1 * (1 - t);

	glm::vec3 b3_0 = b2_1 * t + b2_0 * (1 - t);

	return b3_0;
}
