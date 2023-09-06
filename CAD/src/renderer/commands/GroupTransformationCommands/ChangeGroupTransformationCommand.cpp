#include "ChangeGroupTransformationCommand.h"
#include "..\..\objects\Components\Selectable.h"
#include "..\..\objects\Components\Position.h"

ChangeGroupTransformationCommand::ChangeGroupTransformationCommand(std::shared_ptr<entt::registry> registry,
	std::shared_ptr<SelectionSystem> selectionSystem,
	std::shared_ptr<AdditionalTransformation> additionalTransformation)
	: Command(),
	registry(registry), selectionSystem(selectionSystem), additionalTransformation(additionalTransformation)
{
}

void ChangeGroupTransformationCommand::execute()
{
	auto cursorPos = selectionSystem->GetCursorPosition();
	if (cursorPos.has_value())
		this->registry->ctx().insert_or_assign<AdditionalTransformation>(
			AdditionalTransformation(cursorPos.value(), additionalTransformation->scale, additionalTransformation->rotation));

	auto view = this->registry->view<Selectable>();
	for (auto [entity, selectable] : view.each())
	{
		if (selectable.selected)
			this->registry->patch<Position>(entity);	// notify to recalculate transformation component
	}
}
