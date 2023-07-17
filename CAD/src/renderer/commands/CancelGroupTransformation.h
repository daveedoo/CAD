#pragma once
#include "Command.h"
#include <memory>
#include <entt/entt.hpp>
#include "..\objects\Components\ScaleRotation.h"

class CancelGroupTransformation : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<ScaleRotation> scaleRotation;

public:
	CancelGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<ScaleRotation> scaleRotation);

	virtual void execute() override;
};

