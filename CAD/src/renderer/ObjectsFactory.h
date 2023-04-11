#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

class ObjectsFactory
{
	entt::entity CreatePoint(glm::vec3 position);
	entt::entity CreateTorus(glm::vec3 position, float minorR = 1.f, float majorR = 5.f, int minorSegments = 10, int majorSegments = 10);
};

