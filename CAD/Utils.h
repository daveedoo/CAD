#pragma once
#include <glm\glm.hpp>
#include "src\Camera.h"

static class Utils
{
public:
	static glm::vec3 GetScreenPositionFrom3DCoordinates(const glm::vec3& position, const Camera& camera);
};
