#pragma once
#include "Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\objects\Components\ScaleRotation.h"

class ApplyGroupTransformation : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<ScaleRotation> scaleRotation;

public:
	ApplyGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<ScaleRotation> scaleRotation);

	// Inherited via Command
	virtual void execute() override;
};

