#pragma once
#include "System.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Scaling.h"
#include "..\objects\Components\Rotation.h"
#include "..\objects\Components\Transformation.h"

class TransformationsSystem : public System
{
public:
	TransformationsSystem(std::shared_ptr<entt::registry> registry);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

private:
	// Removes or emlaces the Transformation component depending on the presence of Position/Scaling/Rotation components.
	// If emplaced, calculates it.
	void FixTransfomationComponent(entt::entity entity, const Position* position,
		const Scaling* scaling, const Rotation* rotation, const Transformation* transformation);
	void FixTransfomationComponent_OnConstruct(entt::registry& registry, entt::entity entity);
	void FixTransfomationComponent_OnPositionDestroy(entt::registry& registry, entt::entity entity);
	void FixTransfomationComponent_OnScalingDestroy(entt::registry& registry, entt::entity entity);
	void FixTransfomationComponent_OnRotationDestroy(entt::registry& registry, entt::entity entity);

	// Recalculates the Transformation component based on Position, Scaling, Rotation.
	void UpdateTransformationComponent(entt::registry& registry, entt::entity entity);
};
