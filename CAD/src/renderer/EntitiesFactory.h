#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "objects/Component.h"
#include <optional>
#include "../../Matrix.h"

class EntitiesFactory
{
private:
	const GLfloat SelectedObjectCursor_LineWidth = 2.f;
	const float SelectedObjectCursor_LineLength = 0.5f;

	std::shared_ptr<entt::registry> registry;

public:
	EntitiesFactory(std::shared_ptr<entt::registry> registry);

	entt::entity CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments, glm::vec3 position);
	entt::entity CreatePoint(glm::vec3 position);
	entt::entity CreateCursor(glm::vec3 position, GLfloat lineWidth, float lineLength);
};
