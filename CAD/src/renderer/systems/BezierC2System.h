#pragma once
#include "System.h"

class BezierC2System : public System
{
public:
	BezierC2System(std::shared_ptr<entt::registry> registry);

	// Inherited via System
	void Update(const Camera& camera) override;
	void Render(const Camera& camera) override;
};
