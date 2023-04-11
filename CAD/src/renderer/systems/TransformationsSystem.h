#pragma once
#include "System.h"

class TransformationsSystem : public System
{
public:
	TransformationsSystem(std::shared_ptr<entt::registry> registry);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

private:
	void FixTransfomationComponenets();
};
