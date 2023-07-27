#pragma once
#include "Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\objects\Components\AdditionalTransformation.h"

class CancelGroupTransformation : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<AdditionalTransformation> additionalTransformation;

public:
	CancelGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<AdditionalTransformation> additionalTransformation);

	virtual void execute() override;
};

