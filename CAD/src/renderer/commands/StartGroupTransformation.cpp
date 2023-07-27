#include "StartGroupTransformation.h"

StartGroupTransformation::StartGroupTransformation(std::shared_ptr<entt::registry> registry,
	std::shared_ptr<SelectionSystem> selectionSystem,
	std::shared_ptr<AdditionalTransformation> scaleRotation)
	: Command(), registry(registry), selectionSystem(selectionSystem), additionalTransformation(scaleRotation)
{
}

void StartGroupTransformation::execute()
{
	auto cursorPos = selectionSystem->GetCursorPosition();
	if (cursorPos.has_value())
		this->registry->ctx().emplace<AdditionalTransformation>(cursorPos.value(), additionalTransformation->scale);
}
