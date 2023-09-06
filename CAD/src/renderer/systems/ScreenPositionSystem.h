#pragma once
#include "System.h"
#include "../../Camera.h"
#include "../../ScreenSizeChangeSubscriber.h"
#include "../../CameraSubscriber.h"

class ScreenPositionSystem : public System, public ScreenSizeChangeSubscriber, public CameraSubscriber
{
private:
	Camera& camera;

public:
	ScreenPositionSystem(std::shared_ptr<entt::registry> registry, Camera& camera);

	// Inherited via System
	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

private:
	void SetPosition_ScreenBasedOn3D(entt::registry& registry, entt::entity entity);
	void SetPosition_3DBasedOnScreen(entt::registry& registry, entt::entity entity);
	void AddScreenPositionComponent(entt::registry& registry, entt::entity entity);
	void RemoveScreenPositionComponent(entt::registry& registry, entt::entity entity);

	void UpdateScreenPositions();

	// Inherited via ScreenSizeChangeSubscriber
	virtual void OnScreenSizeChanged(unsigned int width, unsigned int height) override;

	// Inherited via CameraSubscriber
	virtual void OnCameraMove() override;
};
