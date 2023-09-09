#pragma once
#include <glm\glm.hpp>
#include "src\Camera.h"
#include <entt\entt.hpp>

static class Utils
{
public:
	static glm::vec3 GetScreenPositionFrom3DCoordinates(const glm::vec3& position, const Camera& camera);
	static glm::vec3 GetObjectColor(bool isSelected);
	static const glm::vec3 PolylineColor;
	static glm::vec3 GetTranslation(const entt::registry& registry, entt::entity pointEntity);
};
