#include "GUI.h"
#include <glm/gtc/type_ptr.hpp>
#include <format>
#include <imgui_stdlib.h>

GUI::GUI(std::shared_ptr<entt::registry> registry, std::shared_ptr<ObjectsManager> objectsManager)
	: objectsManager(objectsManager), leftPanelHeight(0)
{}

void GUI::RenderMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Torus"))
			{
				objectsManager->AddTorus();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	this->leftPanelHeight = 20;
	this->rightPanelHeight = 20;
}

void GUI::RenderCursorWindow(glm::vec3& position)
{
	ImGui::SetNextWindowPos({0.f, this->leftPanelHeight});
	ImGui::SetNextWindowSize({ GUI_WIDTH, 0.f });

	ImGui::Begin("Cursor");
	ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f);

	ImVec2 wndSize = ImGui::GetWindowSize();
	this->leftPanelHeight += wndSize.y;
	ImGui::End();
}

void GUI::RenderTorusGUI(std::string name, entt::entity torusEntity, TorusComponent& torusComp, Transformation& transf)
{
	ImGui::SetNextWindowPos({ 0.f, this->leftPanelHeight });
	ImGui::SetNextWindowSize({ GUI_WIDTH, 0.f });

	ImGui::Begin(name.c_str());
	RenderTorusTreeNode(torusEntity, torusComp);
	RenderTransformationTreeNode(transf);

	ImVec2 wndSize = ImGui::GetWindowSize();
	this->leftPanelHeight += wndSize.y;
	ImGui::End();
}

void GUI::RenderTorusTreeNode(entt::entity entity, TorusComponent& torusComp)
{
	if (ImGui::TreeNodeEx("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool paramChanged = false;
		if (ImGui::DragFloat("Minor R", &torusComp.minorR, 0.001f, 0.01f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (ImGui::DragFloat("Major R", &torusComp.majorR, 0.001f, 0.01f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (ImGui::DragInt("Minor segments", &torusComp.minorSegments, 1, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (ImGui::DragInt("Major segments", &torusComp.majorSegments, 1, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
		if (paramChanged)
			this->objectsManager->UpdateTorusMesh(entity);

		ImGui::TreePop();
	}
}

void GUI::RenderTransformationTreeNode(Transformation& transf)
{
	if (ImGui::TreeNodeEx("Transformations", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool transformChanged = false;
		if (ImGui::DragFloat3("Translation", glm::value_ptr(transf.translation), 0.01f)) transformChanged = true;
		if (ImGui::DragFloat3("Scale", glm::value_ptr(transf.scale), 0.01f)) transformChanged = true;
		if (ImGui::DragFloat("RotX", &transf.rotX, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) transformChanged = true;
		if (ImGui::DragFloat("RotY", &transf.rotY, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) transformChanged = true;
		if (ImGui::DragFloat("RotZ", &transf.rotZ, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) transformChanged = true;
		if (transformChanged)
		{
			transf.worldMatrix = Matrix::Translation(transf.translation) *
				Matrix::RotationZ(glm::radians(transf.rotZ)) * Matrix::RotationY(glm::radians(transf.rotY)) * Matrix::RotationX(glm::radians(transf.rotX)) *
				Matrix::Scale(transf.scale);
		}

		ImGui::TreePop();
	}
}

void GUI::RenderObjectsList(std::vector<std::tuple<entt::entity, Selectable&>> objects)
{
	auto displayWidth = ImGui::GetIO().DisplaySize.x;
	ImGui::SetNextWindowPos({ displayWidth - GUI_WIDTH, this->rightPanelHeight });
	ImGui::SetNextWindowSize({ GUI_WIDTH, 0.f });

	ImGui::Begin("Entities");
	for (auto& [entity, selectable] : objects)
	{
		RenderObjectSelectable(entity, selectable);
	}

	ImVec2 wndSize = ImGui::GetWindowSize();
	this->rightPanelHeight += wndSize.y;
	ImGui::End();
}

void GUI::RenderObjectSelectable(entt::entity entity, Selectable& selectable)
{
	std::string renamePopup = std::format("Rename {}", selectable.name);
	if (ImGui::Selectable(selectable.name.c_str(), &selectable.selected, ImGuiSelectableFlags_AllowDoubleClick))
	{
		if (ImGui::GetIO().KeyCtrl)
		{
			if (selectable.selected)
				objectsManager->OnObjectSelected(entity);
			else
				objectsManager->OnObjectUnselected(entity);
		}
		else
		{
			objectsManager->UnselectAllObjectsExcept(entity);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				ImGui::OpenPopup(renamePopup.c_str());
		}
	}

	auto disp = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos({disp.x * 0.5f, disp.y * 0.5f}, ImGuiCond_Appearing, {0.5f, 0.5f});
	if (ImGui::BeginPopupModal(renamePopup.c_str(), (bool*)0, ImGuiWindowFlags_AlwaysAutoResize))
	{
		std::string newName = selectable.name;
		ImGui::InputText("Name", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			selectable.name = newName;
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Remove"))
			objectsManager->RemoveEntity(entity);

		if (ImGui::IsKeyPressed(ImGuiKey_Escape))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}
