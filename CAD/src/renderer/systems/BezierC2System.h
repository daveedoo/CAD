#pragma once
#include "System.h"
#include "..\EntitiesFactory.h"

class BezierC2System : public System
{
	std::shared_ptr<EntitiesFactory> entitiesFactory;

public:
	BezierC2System(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory);

	// Inherited via System
	void Update(const Camera& camera) override;
	void Render(const Camera& camera) override;

private:
	void SetBernsteinPoints(entt::registry& registry, entt::entity bezierc2Entity);
	void UpdateAllBeziersContainingDeBoorPoint(entt::registry& registry, entt::entity positionEntity);
};
