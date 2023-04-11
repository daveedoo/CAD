#pragma once
#include "System.h"
#include <memory>
#include <entt/entt.hpp>
#include "../ObjectsManager.h"

class SelectionSystem : public System
{
private:
	const GLfloat Cursor_LineWidth = 2.f;
	const float Cursor_LineLength = 0.5f;

	std::shared_ptr<ObjectsManager> objectsManager;
	entt::entity selectionCursor;

public:
	SelectionSystem(std::shared_ptr<entt::registry> registry, std::shared_ptr<ObjectsManager> objectsManager);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;
	void UpdateCursor();
	std::optional<glm::vec3> GetCursorPosition();

private:
	void SetSelectionCursor(glm::vec3 position);
	void HideSelectionCursor();
};
