#pragma once
#include <imgui.h>
#include <memory>
#include <entt/entt.hpp>
#include "ObjectsManager.h"
#include "objects/Component.h"

class GUI
{
private:
	std::shared_ptr<ObjectsManager> objectsManager;

	const float GUI_WIDTH = 400.f;
	float leftPanelHeight = 0;
	float rightPanelHeight = 0;

public:
	GUI(std::shared_ptr<entt::registry> registry, std::shared_ptr<ObjectsManager> objectsManager);

public:
	void RenderMenu();
	void RenderCursorWindow(entt::entity entity, glm::vec3& position);
	void RenderTorusGUI(std::string name, entt::entity torusEntity, TorusComponent& torusComp, Position& position, ScaleRotation& sr);
	void RenderPointGUI(std::string name, entt::entity pointEntity, Point& point);
	void RenderTorusTreeNode(entt::entity entity, TorusComponent& torusComp);
	void RenderTransformationTreeNode(entt::entity entity, Position& position, ScaleRotation& sr);
	
	void RenderObjectsList(std::vector<std::tuple<entt::entity, Selectable&>> objects);
	void RenderObjectSelectable(entt::entity entity, Selectable& selectable);
};
