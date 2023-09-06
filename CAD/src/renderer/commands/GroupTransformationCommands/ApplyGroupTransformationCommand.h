#pragma once
#include "..\Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\..\objects\Components\AdditionalTransformation.h"

class ApplyGroupTransformationCommand : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<AdditionalTransformation> additionalTransformation;

public:
	ApplyGroupTransformationCommand(std::shared_ptr<entt::registry> registry, std::shared_ptr<AdditionalTransformation> additionalTransformation);

	// Inherited via Command
	virtual void execute() override;
};

