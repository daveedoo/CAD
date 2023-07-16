#pragma once
#include "System.h"
#include "../../Camera.h"
#include "../../ScreenSizeChangeSubscriber.h"

class ScreenPositionSystem : public System, public ScreenSizeChangeSubscriber
{
private:
	Camera& camera;

public:
	ScreenPositionSystem(std::shared_ptr<entt::registry> registry, Camera& camera);

	// Inherited via System
	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

private:
	void SetPositionScreenBasedOn3D(entt::registry& registry, entt::entity entity);
	void AddScreenPositionComponent(entt::registry& registry, entt::entity entity);
	void RemoveScreenPositionComponent(entt::registry& registry, entt::entity entity);

	// Inherited via ScreenSizeChangeSubscriber
	virtual void OnScreenSizeChanged() override;
};
