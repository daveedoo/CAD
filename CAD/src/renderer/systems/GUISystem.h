#pragma once
#include "System.h"
#include "../gui/GUIElement.h"

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

public:
	GUISystem(std::shared_ptr<entt::registry> registry, Scene& scene);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	void AddGroupWindow(std::unique_ptr<GUIElement> groupGuiWindow);

private:
	unsigned int GetSelectedEntitiesCount();
	void SetDirty(entt::entity entity);

	void RenderWindowOnLeft(std::string windowName, GUIElement& guiElement);
	void RenderWindowOnLeft(std::string windowName, const std::function<void()>& inside);
	void RenderWindowOnRight(std::string windowName, const std::function<void()>& inside);
	void PopupCentered(std::string windowName, const std::function<void()>& inside);

	void RenderMainMenuBar();
	void RenderCursorWindow(entt::entity entity, glm::vec3& position);
	void RenderEntitiesList();
	void RenderEntitiesDetailsWindow();
	void RenderTorusTreeNode(entt::entity entity, TorusComponent& torusComp);
	void RenderTransformationsTreeNode(entt::entity entity, Position* position, ScaleRotation* sr);
};
