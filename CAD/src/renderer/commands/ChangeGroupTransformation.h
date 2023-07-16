#pragma once
#include "Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "../systems/SelectionSystem.h"

class ChangeGroupTransformation : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<SelectionSystem> selectionSystem;
	std::shared_ptr<ScaleRotation> scaleRotation;

public:
	ChangeGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<SelectionSystem> selectionSystem, std::shared_ptr<ScaleRotation> scaleRotation);
	virtual void execute() override;
};