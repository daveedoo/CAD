#pragma once
#include "System.h"
#include <memory>
#include <entt/entt.hpp>
#include "../EntitiesFactory.h"

class SelectionSystem : public System
{
private:
	const GLfloat Cursor_LineWidth = 2.f;
	const float Cursor_LineLength = 0.5f;

	std::shared_ptr<EntitiesFactory> entitiesFactory;
	entt::entity selectionCursor;

public:
	SelectionSystem(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;
	void UpdateCursor();
	std::optional<glm::vec3> GetCursorPosition();
};
