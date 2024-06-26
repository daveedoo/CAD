#include "StartGroupTransformationCommand.h"

StartGroupTransformationCommand::StartGroupTransformationCommand(std::shared_ptr<entt::registry> registry,
	std::shared_ptr<SelectionSystem> selectionSystem,
	std::shared_ptr<AdditionalTransformation> additionalTransformation)
	: Command(), registry(registry), selectionSystem(selectionSystem), additionalTransformation(additionalTransformation)
{
}

void StartGroupTransformationCommand::execute()
{
	auto cursorPos = selectionSystem->GetCursorPosition();
	if (cursorPos.has_value())
		this->registry->ctx().emplace<AdditionalTransformation>(cursorPos.value(), additionalTransformation->scale);
}
