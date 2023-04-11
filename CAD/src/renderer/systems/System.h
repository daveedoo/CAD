#pragma once
#include <memory>
#include <entt/entt.hpp>
#include "../../Camera.h"
#include "../objects/Component.h"

class System
{
protected:
	std::shared_ptr<entt::registry> registry;

public:
	System(std::shared_ptr<entt::registry> registry) :
		registry(registry) {}

	virtual void Update(const Camera& camera) = 0;
	virtual void Render(const Camera& camera) = 0;
};
