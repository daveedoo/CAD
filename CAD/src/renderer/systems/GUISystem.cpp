#include "GUISystem.h"
#include <imgui_stdlib.h>
#include <iostream>
#include "../Scene.h"

GUISystem::GUISystem(std::shared_ptr<entt::registry> registry, Scene& scene) : System(registry),
	scene(scene)
{}

unsigned int GUISystem::GetSelectedEntitiesCount()
{
	int count = 0;
	auto view = this->registry->view<Selectable, Position>();
	for (auto [entity, selectable, position] : view.each())
	{
		if (selectable.selected)
			count++;
	}

	return count;
}

void GUISystem::SetDirty(entt::entity entity)
{
	this->registry->emplace_or_replace<Dirty>(entity);
}

void GUISystem::RenderWindowOnLeft(std::string windowName, GUIElement& guiElement)
{
	ImGui::SetNextWindowPos({ 0.f, this->leftPanelHeight });
	ImGui::SetNextWindowSize({ GUI_WIDTH, 0.f });
	ImGui::Begin(windowName.c_str());

	guiElement.Draw();

	ImVec2 wndSize = ImGui::GetWindowSize();
	this->leftPanelHeight += wndSize.y;
	ImGui::End();
}

void GUISystem::Update(const Camera& camera)
{
}

void GUISystem::Render(const Camera& camera)
{
	this->leftPanelHeight = 0;
	this->rightPanelHeight = 0;

	RenderMainMenuBar();
		
	// main cursor
	auto mainCursor = scene.GetMainCursor();
	auto& position = this->registry->get<Position>(mainCursor);
	RenderCursorWindow(mainCursor, position.position);

	RenderEntitiesList();
	if (GetSelectedEntitiesCount() > 1)
		RenderWindowOnLeft("Group Transformation", *this->groupGuiWindow);
	else
		RenderEntitiesDetailsWindow();
}

void GUISystem::AddGroupWindow(std::unique_ptr<GUIElement> groupGuiWindow)
{
	this->groupGuiWindow = std::move(groupGuiWindow);
}

void GUISystem::RenderWindowOnLeft(std::string windowName, const std::function<void()>& inside)
{
	ImGui::SetNextWindowPos({ 0.f, this->leftPanelHeight });
	ImGui::SetNextWindowSize({ GUI_WIDTH, 0.f });
	ImGui::Begin(windowName.c_str());

	inside();

	ImVec2 wndSize = ImGui::GetWindowSize();
	this->leftPanelHeight += wndSize.y;
	ImGui::End();
}

void GUISystem::RenderWindowOnRight(std::string windowName, const std::function<void()>& inside)
{
	float displayWidth = ImGui::GetIO().DisplaySize.x;
	ImGui::SetNextWindowPos({ displayWidth - GUI_WIDTH, this->rightPanelHeight });
	ImGui::SetNextWindowSize({ GUI_WIDTH, 0.f });
	ImGui::Begin(windowName.c_str());

	inside();

	ImVec2 wndSize = ImGui::GetWindowSize();
	this->rightPanelHeight += wndSize.y;
	ImGui::End();
}

void GUISystem::PopupCentered(std::string windowName, const std::function<void()>& inside)
{
	auto disp = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos({disp.x * 0.5f, disp.y * 0.5f}, ImGuiCond_Appearing, {0.5f, 0.5f});
	if (ImGui::BeginPopupModal(windowName.c_str(), (bool*)0, ImGuiWindowFlags_AlwaysAutoResize))
	{
		inside();
		ImGui::EndPopup();
	}

}

void GUISystem::RenderMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Point"))
				scene.AddPoint();
			if (ImGui::MenuItem("Torus"))
				scene.AddTorus();
			ImGui::EndMenu();
		}
		ImGui::Text(std::format("entities: {}", this->registry->size()).c_str());

		ImGui::EndMainMenuBar();
	}
	this->leftPanelHeight += 20;
	this->rightPanelHeight += 20;
}



void GUISystem::RenderCursorWindow(entt::entity entity, glm::vec3& position)
{
	RenderWindowOnLeft("Cursor", [&]() -> void
		{
			if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f))
				SetDirty(entity);
		});
}

void GUISystem::RenderEntitiesList()
{
	auto selectablesView = this->registry->view<Selectable>();

	static std::function<void(entt::entity)> unselectAllExcept = [&](entt::entity ent) -> void
	{
		for (auto [entity, selectable] : selectablesView.each())
		{
			if (entity != ent)
				selectable.selected = false;
		}
	};

	// TODO: all selection logic to be in one system
	static std::function<void()> selectionChanged = [&]() -> void
	{
		int count = 0;
		auto cursorPos = glm::vec3(0.f);

		auto view = this->registry->view<Selectable, Position>();
		for (auto [entity, selectable, position] : view.each())
		{
			if (selectable.selected)
			{
				scene.SetSelected(entity);

				cursorPos += position.position;
				count++;
			}
			else
			{
				scene.SetUnselected(entity);
			}
		}
	};

	RenderWindowOnRight("Entity list", [&]() -> void
		{
			for (auto [entity, selectable] : selectablesView.each())
			{
				std::string renamePopup = std::format("Rename {}", selectable.name);
				if (ImGui::Selectable(selectable.name.c_str(), &selectable.selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (!ImGui::GetIO().KeyCtrl)
						unselectAllExcept(entity);

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						ImGui::OpenPopup(renamePopup.c_str());

					selectionChanged();
				}

				PopupCentered(renamePopup, [&]() -> void
					{
						std::string newName = selectable.name;
						ImGui::InputText("Name", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
						if (ImGui::IsItemDeactivatedAfterEdit())
						{
							selectable.name = newName;
							ImGui::CloseCurrentPopup();
						}

						if (ImGui::Button("Remove"))
							scene.RemoveEntity(entity);

						if (ImGui::IsKeyPressed(ImGuiKey_Escape))
							ImGui::CloseCurrentPopup();
					});
			}
		});
}

void GUISystem::RenderEntitiesDetailsWindow()
{
	auto view = this->registry->view<Selectable>();
	RenderWindowOnLeft("Entities", [&]() -> void
		{
			for (auto [entity, selectable] : view.each())
			{
				if (!selectable.selected)
					continue;

				if (this->registry->any_of<TorusComponent, Position, ScaleRotation, BezierC0>(entity))
				{
					if (ImGui::TreeNodeEx(selectable.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						auto [torusComp, position, scaleRot, bezier_c0] = this->registry->try_get<TorusComponent, Position, ScaleRotation, BezierC0>(entity);	// TODO: duplicated type parameters
						if (torusComp != nullptr)
							RenderTorusTreeNode(entity, *torusComp);
						if (position != nullptr || scaleRot != nullptr)
							RenderTransformationsTreeNode(entity, position, scaleRot);
						if (bezier_c0 != nullptr)
							RenderBezierC0TreeNode(entity, *bezier_c0);
						ImGui::TreePop();
					}
				}
			}
		});
}

void GUISystem::RenderTorusTreeNode(entt::entity entity, TorusComponent& torusComp)
{
	if (ImGui::TreeNodeEx("Parameters"))
	{
		bool paramChanged = false;
		if (ImGui::DragFloat("Minor R", &torusComp.minorR, 0.001f, 0.01f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (ImGui::DragFloat("Major R", &torusComp.majorR, 0.001f, 0.01f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (ImGui::DragInt("Minor segments", &torusComp.minorSegments, 0.1f, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (ImGui::DragInt("Major segments", &torusComp.majorSegments, 0.1f, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (paramChanged)
			this->registry->patch<TorusComponent>(entity);

		ImGui::TreePop();
	}
}

void GUISystem::RenderTransformationsTreeNode(entt::entity entity, Position* position, ScaleRotation* sr)
{
	if (ImGui::TreeNodeEx("Local transformations", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool positionChanged = false;
		bool scaleRotChanged = false;
		if (position != nullptr)
		{
			if (ImGui::DragFloat3("Position", glm::value_ptr(position->position), 0.01f)) positionChanged = true;
			if (positionChanged)
				this->registry->patch<Position>(entity);
		}
		if (sr != nullptr)
		{
			if (ImGui::DragFloat3("Scale", glm::value_ptr(sr->scale), 0.01f)) scaleRotChanged = true;
			if (ImGui::DragFloat("Axis Lambda", &sr->axisLambda, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) scaleRotChanged = true;
			if (ImGui::DragFloat("Axis Fi", &sr->axisFi, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) scaleRotChanged = true;
			if (ImGui::DragFloat("Angle", &sr->angle, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) scaleRotChanged = true;
			if (scaleRotChanged)
				this->registry->patch<ScaleRotation>(entity);
		}
		if (positionChanged || scaleRotChanged)
			SetDirty(entity);

		ImGui::TreePop();
	}
}

void GUISystem::RenderBezierC0TreeNode(entt::entity entity, const BezierC0& bezier)
{
	if (ImGui::TreeNodeEx("Points:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (const auto& point : bezier.points)
		{
			const auto& selectable = this->registry->get<Selectable>(point);
			ImGui::Selectable(selectable.name.c_str());
		}
		ImGui::TreePop();
	}
}
