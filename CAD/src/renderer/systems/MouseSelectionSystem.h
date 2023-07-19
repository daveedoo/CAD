#pragma once
#include "System.h"
#include "../../Window/input/handlers/InputHandler.h"

class MouseSelectionSystem : public System, public InputHandler
{
private:
	std::shared_ptr<Camera> camera;

public:
	MouseSelectionSystem(std::shared_ptr<entt::registry> registry, std::shared_ptr<Camera> camera);

	// Inherited via System
	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	// Inherited via InputHandler
	virtual void ProcessInput(const InputEvent& event) override;
};
