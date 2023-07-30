#include "GUISystem.h"
#include <imgui_stdlib.h>
#include <iostream>
#include "../Scene.h"
#include <algorithm>
#include "..\objects\Components\Position.h"
#include "..\objects\Components\ScreenPosition.h"
#include <glm\gtc\type_ptr.hpp>
#include "..\objects\Components\TorusComponent.h"
#include "..\objects\Components\Rotation.h"
#include "..\objects\Components\Scaling.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Point.h"
#include "..\gui\widgets\Widgets.h"

GUISystem::GUISystem(std::shared_ptr<entt::registry> registry,
	std::unique_ptr<GUIElement> mainMenuBar, 
	Scene& scene)
	: System(registry),
	  mainMenuBar(std::move(mainMenuBar)),
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
	auto [pos3d, screenPos] = this->registry->get<Position, ScreenPosition>(mainCursor);
	RenderCursorWindow(mainCursor, pos3d.position, screenPos.position);

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

void GUISystem::RenderMainMenuBar()
{
	this->mainMenuBar->Draw();

	this->leftPanelHeight += 20;
	this->rightPanelHeight += 20;
}



void GUISystem::RenderCursorWindow(entt::entity entity, glm::vec3& pos3d, glm::vec3& screenPos)
{
	RenderWindowOnLeft("3D Cursor", [&]() -> void
		{
			if (ImGui::DragFloat3("3D Position", glm::value_ptr(pos3d), 0.01f))
			{
				this->registry->patch<Position>(entity);
			}
			if (ImGui::DragFloat2("Screen Position", glm::value_ptr(screenPos), 0.1f))
			{
				this->registry->patch<ScreenPosition>(entity);
			}
		});
}

void GUISystem::RenderEntitiesList()
{
	auto selectablesView = this->registry->view<Selectable>();

	static std::function<void(entt::entity)> unselectAllExcept = [&](entt::entity ent) -> void
	{
		for (auto [entity, selectable] : selectablesView.each())
		{
			if (entity != ent && selectable.selected == true)
			{
				this->registry->patch<Selectable>(entity, [](Selectable& selectbl) -> void
					{
						selectbl.selected = false;
					});
			}
		}
	};

	auto beziersView = this->registry->view<BezierC0, Selectable>();
	for (auto [entity, bezier, bezierSelect] : beziersView.each())
	{
		//if (bezierSelect.selected)
	}

	RenderWindowOnRight("Entity list", [&]() -> void
		{
			for (auto [entity, selectable] : selectablesView.each())
			{
				if (ImGui::Selectable(selectable.name.c_str(), &selectable.selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (!ImGui::GetIO().KeyCtrl)
						unselectAllExcept(entity);
					this->registry->patch<Selectable>(entity);
				}
				if (ImGui::BeginPopupContextItem())
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
					ImGui::EndPopup();
				}
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

				if (this->registry->any_of<TorusComponent, Position, Scaling, Rotation, BezierC0>(entity))
				{
					if (ImGui::TreeNodeEx(selectable.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						auto [torusComp, position, scaling, rotation, bezier_c0] = this->registry->try_get<TorusComponent, Position, Scaling, Rotation, BezierC0>(entity);	// TODO: duplicated type parameters
						if (torusComp != nullptr)
							RenderTorusTreeNode(entity, *torusComp);
						if (position != nullptr || scaling != nullptr || rotation != nullptr)
							RenderTransformationsTreeNode(entity, position, scaling, rotation);
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

void GUISystem::RenderTransformationsTreeNode(entt::entity entity, Position* position, Scaling* scaling, Rotation* rotation)
{
	if (ImGui::TreeNodeEx("Local transformations", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (position != nullptr)
		{
			bool positionChanged = false;
			if (ImGui::DragFloat3("Position", glm::value_ptr(position->position), 0.01f)) positionChanged = true;
			if (positionChanged)
				this->registry->patch<Position>(entity);
		}
		if (scaling != nullptr)
		{
			if (ImGui::DragFloat3("Scale", glm::value_ptr(scaling->scale), 0.01f))
				this->registry->patch<Scaling>(entity);
		}
		if (rotation != nullptr)
		{
			if (ImGui::RotationRPY(*rotation))
				this->registry->patch<Rotation>(entity);
		}

		ImGui::TreePop();
	}
}

void GUISystem::RenderBezierC0TreeNode(entt::entity entity, const BezierC0& bezier)
{
	static const std::string NEW_BEZIER_POINT_PayloadID = "BEZIER_C0_NEW_POINT_ID";
	// helper function
	std::function<void(size_t, size_t)> replaceValues = [&](size_t idx1, size_t idx2) -> void
	{
		this->registry->patch<BezierC0>(entity, [&](BezierC0& patchBezier)
			{
				auto val1 = patchBezier.points[idx1];
				patchBezier.points[idx1] = patchBezier.points[idx2];
				patchBezier.points[idx2] = val1;
			});
	};


	if (ImGui::BeginListBox("Curve points", ImVec2(-FLT_MIN, 0.f)))
	{
		ImGui::SeparatorText(std::format("{} curve points:", bezier.points.size()).c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(NEW_BEZIER_POINT_PayloadID.c_str()))
			{
				entt::entity movedPointEntity = *static_cast<entt::entity*>(payload->Data);
				this->registry->patch<BezierC0>(entity, [&](BezierC0& bezierToAddPoint) -> void
					{
						bezierToAddPoint.points.push_back(movedPointEntity);
					});
			}
			ImGui::EndDragDropTarget();
		}

		for (auto it = bezier.points.begin(); it != bezier.points.end(); it++)
		{
			// TODO: this bool may be removed if using std::list<> instead of std::vector<>
			// also, continuous dragging might be possible to implement then.
			// However, due to the component constness, efficiency wouldn't be much better
			bool iteratorInvalid = false;

			const auto& selectable = this->registry->get<Selectable>(*it);
			ImGui::PushID(&(*it));
			ImGui::Selectable(selectable.name.c_str());

			// reordering drag and drop inside "Points" list
			if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
			{
				int it_advance = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).y < 0.f ? -1 : 1;
				if (it != bezier.points.begin() || it_advance > 0)
				{
					auto it2 = it + it_advance;
					if (it2 >= bezier.points.begin() && it2 < bezier.points.end())
					{
						int it1_idx = std::distance(bezier.points.begin(), it);
						int it2_idx = std::distance(bezier.points.begin(), it2);
						replaceValues(it1_idx, it2_idx);
						ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
					}
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				int currentIdx = it - bezier.points.begin();
				if (currentIdx > 0)
				{
					if (ImGui::Button("Up", ImVec2(-FLT_MIN, 0.f)))
					{
						replaceValues(currentIdx, (size_t)currentIdx - 1);
						ImGui::CloseCurrentPopup();
						iteratorInvalid = true;
					}
				}
				if (bezier.points.end() - it > 1)
				{
					if (ImGui::Button("Down", ImVec2(-FLT_MIN, 0.f)))
					{
						replaceValues(currentIdx, static_cast<size_t>(currentIdx) + 1);
						ImGui::CloseCurrentPopup();
						iteratorInvalid = true;
					}
				}
				if (ImGui::Button("Remove from curve"))
				{
					this->registry->patch<BezierC0>(entity, [&](BezierC0& patchBezier) -> void
						{
							patchBezier.points.erase(it);
						});
					ImGui::CloseCurrentPopup();
					iteratorInvalid = true;
				}

				ImGui::EndPopup();
			}

			ImGui::PopID();
			if (iteratorInvalid) break;
		}

		ImGui::EndListBox();
	}

	if (ImGui::BeginListBox("All points", ImVec2(-FLT_MIN, 0.f)))
	{
		ImGui::SeparatorText("All points:");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
			ImGui::SetTooltip("Double click to add to the list above");

		auto view = this->registry->view<Point, Selectable>();
		for (auto [pointEntity, selectable] : view.each())
		{
			if (ImGui::Selectable(selectable.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					this->registry->patch<BezierC0>(entity, [&](BezierC0& bezier) -> void
						{
							bezier.points.push_back(pointEntity);
						});
				}
			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload(NEW_BEZIER_POINT_PayloadID.c_str(), &pointEntity, sizeof(pointEntity));
				ImGui::Text(selectable.name.c_str());

				ImGui::EndDragDropSource();
			}
		}
		ImGui::EndListBox();
	}
}
