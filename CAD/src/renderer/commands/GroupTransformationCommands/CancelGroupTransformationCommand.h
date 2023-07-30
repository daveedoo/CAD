#pragma once
#include "..\Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\..\objects\Components\AdditionalTransformation.h"

class CancelGroupTransformationCommand : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<AdditionalTransformation> additionalTransformation;

public:
	CancelGroupTransformationCommand(std::shared_ptr<entt::registry> registry, std::shared_ptr<AdditionalTransformation> additionalTransformation);

	virtual void execute() override;
};

