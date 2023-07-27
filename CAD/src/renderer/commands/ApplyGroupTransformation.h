#pragma once
#include "Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\objects\Components\AdditionalTransformation.h"

class ApplyGroupTransformation : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<AdditionalTransformation> additionalTransformation;

public:
	ApplyGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<AdditionalTransformation> additionalTransformation);

	// Inherited via Command
	virtual void execute() override;
};

