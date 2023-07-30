#pragma once
#include "..\Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\../systems/SelectionSystem.h"
#include "..\..\objects\Components\AdditionalTransformation.h"

class StartGroupTransformationCommand : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<SelectionSystem> selectionSystem;
	std::shared_ptr<AdditionalTransformation> additionalTransformation;

public:
	StartGroupTransformationCommand(std::shared_ptr<entt::registry> registry,
		std::shared_ptr<SelectionSystem> selectionSystem,
		std::shared_ptr<AdditionalTransformation> additionalTransformation);

	virtual void execute() override;
};
