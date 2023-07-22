#include "SelectionSystem.h"
#include "..\objects\Components\Dirty.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"

SelectionSystem::SelectionSystem(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory) : System(registry),
	entitiesFactory(entitiesFactory),
	selectionCursor(entitiesFactory->CreateCursor(glm::vec3(0.f), Cursor_LineWidth, Cursor_LineLength))
{
	HideSelectionCursor();

	this->registry->on_update<Selectable>().connect<&SelectionSystem::UpdateCursor>(*this);
}

void SelectionSystem::Update(const Camera& camera)
{
}

void SelectionSystem::Render(const Camera& camera)
{
}

// force update. basically is called when selection state of an entity was changed
void SelectionSystem::UpdateCursor()
{
	int count = 0;
	auto cursorPos = glm::vec3(0.f);

	auto view = this->registry->view<Selectable, Position>();
	for (auto [entity, selectable, position] : view.each())
	{
		if (selectable.selected)
		{
			cursorPos += position.position;
			count++;
		}
	}

	if (count > 1)
	{
		cursorPos /= count;
		SetSelectionCursor(cursorPos);
	}
	else
	{
		HideSelectionCursor();
	}
}

void SelectionSystem::SetSelectionCursor(glm::vec3 position)
{
	this->registry->emplace_or_replace<Position>(selectionCursor, position);
	this->registry->emplace_or_replace<Dirty>(selectionCursor);
}

std::optional<glm::vec3> SelectionSystem::GetCursorPosition()
{
	auto pos = this->registry->try_get<Position>(selectionCursor);
	if (pos != nullptr)
		return pos->position;
	else
		return std::nullopt;
}

void SelectionSystem::HideSelectionCursor()
{
	this->registry->remove<Position>(selectionCursor);
	this->registry->emplace_or_replace<Dirty>(selectionCursor);
}
