#include "ChangeGroupTransformation.h"
#include "../objects/Component.h"

ChangeGroupTransformation::ChangeGroupTransformation(std::shared_ptr<entt::registry> registry,
	std::shared_ptr<SelectionSystem> selectionSystem,
	std::shared_ptr<ScaleRotation> scaleRotation)
	: Command(),
	registry(registry), selectionSystem(selectionSystem), scaleRotation(scaleRotation)
{
}

void ChangeGroupTransformation::execute()
{
	auto cursorPos = selectionSystem->GetCursorPosition();
	if (cursorPos.has_value())
		this->registry->ctx().insert_or_assign<AdditionalTransformation>(AdditionalTransformation(cursorPos.value(), *scaleRotation));

	auto view = this->registry->view<Selectable, Position>();
	for (auto [entity, selectable, position] : view.each())
	{
		if (selectable.selected)
		{
			this->registry->emplace<Dirty>(entity);
		}
	}
}
