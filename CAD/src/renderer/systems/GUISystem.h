#pragma once
#include "System.h"
#include "../gui/GUIElement.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\TorusComponent.h"
#include "..\objects\Components\BezierC0.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\ScaleRotation.h"

class Scene;

class GUISystem : public System
{
private:
	Scene& scene;

	const float GUI_WIDTH = 400.f;
	float leftPanelHeight = 0;
	float rightPanelHeight = 0;

	std::unique_ptr<GUIElement> groupButtonWindow;
	std::unique_ptr<GUIElement> groupGuiWindow;
	std::unique_ptr<GUIElement> mainMenuBar;

public:
	GUISystem(std::shared_ptr<entt::registry> registry,
		std::unique_ptr<GUIElement> mainMenuBar,
		Scene & scene);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	void AddGroupWindow(std::unique_ptr<GUIElement> groupGuiWindow);

private:
	unsigned int GetSelectedEntitiesCount();

	void RenderWindowOnLeft(std::string windowName, GUIElement& guiElement);
	void RenderWindowOnLeft(std::string windowName, const std::function<void()>& inside);
	void RenderWindowOnRight(std::string windowName, const std::function<void()>& inside);

	void RenderMainMenuBar();
	void RenderCursorWindow(entt::entity entity, glm::vec3& position, glm::vec3& screenPos);
	void RenderEntitiesList();
	void RenderEntitiesDetailsWindow();
	void RenderTorusTreeNode(entt::entity entity, TorusComponent& torusComp);
	void RenderTransformationsTreeNode(entt::entity entity, Position* position, ScaleRotation* sr);
	void RenderBezierC0TreeNode(entt::entity entity, const BezierC0& bezier);
};
