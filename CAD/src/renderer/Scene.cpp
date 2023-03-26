#include "Scene.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Config.h"
#include <filesystem>
#include "../gl/wrappers/Shader.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "../Window/input/events/ResizeEvent.h"
#include <imgui.h>
#include <glm/gtx/rotate_vector.hpp>
#include <imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../Matrix.h"


Scene::Scene(unsigned int frame_width, unsigned int frame_height) :
	camera(std::make_unique<Camera>(90, static_cast<float>(frame_width) / static_cast<float>(frame_height), 0.1f, 100.f)),
	cameraMovementHandler(std::make_unique<CameraMovementInputHandler>(*this->camera)),
	floor(std::make_unique<Floor>(50, 50)),
	cursor(std::make_unique<Cursor>()),
	registry(std::make_unique<entt::registry>())
{
	this->camera->Scale(1.f / 10.f);

	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "torus.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "torus.frag");
	this->torusProgram = std::make_unique<GL::Program>(vertexShader, fragmentShader);
	this->torusProgram->SetVec3("color", glm::vec3(1.f));

	CreateTorus(1.f, 3.f, 10, 10);
	CreateTorus(1.f, 10.f, 20, 20);
}

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->camera->SetAspect(static_cast<float>(width) / static_cast<float>(height));
}

void Scene::CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments)
{
	static unsigned int counter = 0;

	const auto entt = registry->create();
	TorusComponent values(minorR, majorR, minorSegments, majorSegments);
	Mesh mesh = Mesh::Torus(values);

	this->registry->emplace<TorusComponent>(entt, values);
	this->registry->emplace<Mesh>(entt, std::move(mesh));
	this->registry->emplace<Selectable>(entt, std::format("Torus {}", ++counter));
	this->registry->emplace<Transformation>(entt);
}

void Scene::Update()
{
}

void Scene::Render()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: make it to ECS
	this->floor->Render(*this->camera);
	this->cursor->Render(*this->camera);

	this->torusProgram->Use();
	this->torusProgram->SetMat4("viewMatrix", camera->GetViewMatrix());
	this->torusProgram->SetMat4("projMatrix", camera->GetProjectionMatrix());

	torus_system();
	namedEntities_system();
}

void Scene::torus_system()
{
	static const float GUI_WIDTH = 400.f;

	glLineWidth(1.f);
	auto view = this->registry->view<TorusComponent, Mesh, Selectable, Transformation>();
	for (auto [entt, torusComp, renderComp, selectable, T] : view.each())
	{
		renderComp.vao->Bind();
		this->torusProgram->SetMat4("worldMatrix", T.worldMatrix);
		glDrawElements(GL_LINES, 4 * torusComp.minorSegments * torusComp.majorSegments, static_cast<GLenum>(renderComp.ebo->GetDataType()), static_cast<void*>(0));

		// =========== GUI ===========
		if (selectable.selected)
		{
			ImGui::Begin(selectable.name.c_str());
			if (ImGui::TreeNodeEx("Parameters"))
			{
				bool paramChanged = false;
				if (ImGui::DragFloat("Minor R", &torusComp.minorR, 0.001f, 0.01f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
				if (ImGui::DragFloat("Major R", &torusComp.majorR, 0.001f, 0.01f, 0.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
				if (ImGui::DragInt("Minor segments", &torusComp.minorSegments, 1, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
				if (ImGui::DragInt("Major segments", &torusComp.majorSegments, 1, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp)) paramChanged = true;
				if (paramChanged)
					registry->replace<Mesh>(entt, Mesh::Torus(torusComp));

				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Transformations"))
			{
				bool transformChanged = false;
				if (ImGui::DragFloat3("Scale", glm::value_ptr(T.scale), 0.01f)) transformChanged = true;
				if (ImGui::DragFloat("RotX", &T.rotX, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) transformChanged = true;
				if (ImGui::DragFloat("RotY", &T.rotY, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) transformChanged = true;
				if (ImGui::DragFloat("RotZ", &T.rotZ, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) transformChanged = true;
				if (ImGui::DragFloat3("Translation", glm::value_ptr(T.translation), 0.01f)) transformChanged = true;
				if (transformChanged)
				{
					T.worldMatrix = Matrix::Translation(T.translation) *
						Matrix::RotationZ(glm::radians(T.rotZ)) * Matrix::RotationY(glm::radians(T.rotY)) * Matrix::RotationX(glm::radians(T.rotX)) *
						Matrix::Scale(T.scale);
				}

				ImGui::TreePop();
			}
			ImGui::End();
		}
	}
}

void Scene::namedEntities_system()
{
	ImGui::Begin("Entities");
	auto view = this->registry->view<Selectable>();
	for (auto [entt, name] : view.each())
	{
		std::string renamePopup = std::format("Renamee {}", name.name);

		if (ImGui::Selectable(name.name.c_str(), &name.selected, ImGuiSelectableFlags_AllowDoubleClick)
			&& !ImGui::GetIO().KeyCtrl)
		{
			for (auto [entity, name] : view.each())
				name.selected = false;
			name.selected = true;

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				ImGui::OpenPopup(renamePopup.c_str());
		
		}
		if (ImGui::BeginPopupModal(renamePopup.c_str(), (bool*)0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string newName = name.name;
			ImGui::InputText("Name", &newName, ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				name.name = newName;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Escape))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
