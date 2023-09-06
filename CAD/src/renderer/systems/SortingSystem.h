#pragma once
#include "System.h"

class SortingSystem : public System
{
public:
	SortingSystem(std::shared_ptr<entt::registry> registry);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	void SortSelectables();
};
