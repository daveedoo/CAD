#include "MainMenuBar.h"
#include "../Scene.h"
#include "../../renderer/objects/Components/Selectable.h"
#include "../../renderer/objects/Components/Point.h"

MainMenuBar::MainMenuBar(Scene& scene,
	std::shared_ptr<entt::registry> registry,
	std::shared_ptr<EntitiesFactory> entitiesFactory,
	std::shared_ptr<Command> addPointCommand)
	: GUIElement(),
	scene(scene), registry(registry), entitiesFactory(entitiesFactory), addPointCommand(addPointCommand)
{
}

//void MainMenuBar::AddMenuItem(std::string label, std::shared_ptr<Command> command)
//{
//}

std::vector<entt::entity> MainMenuBar::GetSelectedPoints()
{
	std::vector<entt::entity> points;

	auto view = this->registry->view<Selectable, Point>();
	for (auto [entity, selectable] : view.each())
	{
		if (selectable.selected)
			points.push_back(entity);
	}

	return points;
}

void MainMenuBar::Draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem("Point"))
			this->addPointCommand->execute();
		if (ImGui::MenuItem("Torus"))
			scene.AddTorus();

		auto selectedPoints = GetSelectedPoints();
		if (ImGui::MenuItem("Bezier C0", nullptr, nullptr, true))
			this->entitiesFactory->CreateBezierC0(selectedPoints);
		if (ImGui::MenuItem("Bezier C2", nullptr, nullptr, true))
			this->entitiesFactory->CreateBezierC2(selectedPoints);

		ImGui::EndMainMenuBar();
	}
}
