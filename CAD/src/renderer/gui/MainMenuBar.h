#pragma once
#include "GUIElement.h"
#include <memory>
#include "../commands/Command.h"
#include <map>
#include <entt/entt.hpp>

class Scene;

class MainMenuBar : public GUIElement
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<Command> addPointCommand;
	Scene& scene;

public:
	MainMenuBar(Scene& scene, std::shared_ptr<entt::registry> registry, std::shared_ptr<Command> addPointCommand);

	//void AddMenuItem(std::string label, std::shared_ptr<Command> command);
	virtual void Draw() override;

private:
	std::vector<entt::entity> GetSelectedPoints();

};
