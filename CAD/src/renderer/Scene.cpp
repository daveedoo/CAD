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
	registry(std::make_shared<entt::registry>()),
	objectsManager(std::make_shared<ObjectsManager>(this->registry)),
	gui(std::make_unique<GUI>(this->registry, this->objectsManager))
{
	this->camera->Scale(1.f / 10.f);

	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "torus.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "torus.frag");
	this->torusProgram = std::make_unique<GL::Program>(vertexShader, fragmentShader);
	this->torusProgram->SetVec3("color", glm::vec3(1.f));

	GL::Shader cursorVS(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "coloredVertices.vert");
	GL::Shader cursorFS(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "coloredVertices.frag");
	this->cursorProgram = std::make_unique<GL::Program>(cursorVS, cursorFS);

	this->pointsVao = std::make_unique<GL::VAO>();
	this->pointsVao->DefineFloatAttribute(*this->objectsManager->pointsVBO, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);

	this->objectsManager->CreateTorus(1.f, 3.f, 10, 10, glm::vec3(0.f));
	this->objectsManager->CreateTorus(1.f, 10.f, 20, 20, glm::vec3(0.f));
}

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->camera->SetAspect(static_cast<float>(width) / static_cast<float>(height));
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

	this->gui->RenderMenu();
	cursors_system();
	torus_system();
	namedEntities_system();
	points_system();
}

glm::vec3 GetObjectColor(bool isSelected)
{
	return isSelected ? glm::vec3(1.f, 0.65f, 0.f) : glm::vec3(1.f, 1.f, 1.f);
}

// renders toruses, uses their own meshes
void Scene::torus_system()
{
	this->torusProgram->SetMat4("viewMatrix", camera->GetViewMatrix());
	this->torusProgram->SetMat4("projMatrix", camera->GetProjectionMatrix());
	glLineWidth(1.f);

	const unsigned int selectedCount = this->objectsManager->GetSelectedEntitiesCount();
	
	auto view = this->registry->view<TorusComponent, Mesh, Selectable, Position, ScaleRotation, Transformation>();
	for (auto [entt, torusComp, mesh, selectable, position, sr, transf] : view.each())
	{
		mesh.vao->Bind();
		this->torusProgram->Use();
		this->torusProgram->SetMat4("worldMatrix", transf.worldMatrix);
		this->torusProgram->SetVec3("color", GetObjectColor(selectable.selected));
		glDrawElements(GL_LINES, 4 * torusComp.minorSegments * torusComp.majorSegments, static_cast<GLenum>(mesh.ebo->GetDataType()), static_cast<void*>(0));

		if (selectable.selected && selectedCount == 1)
		{
			this->gui->RenderTorusGUI(selectable.name, entt, torusComp, position, sr);
		}
	}
}

// GUI list only
void Scene::namedEntities_system()
{
	std::vector<std::tuple<entt::entity, Selectable&>> objects;

	auto view = this->registry->view<Selectable>();
	for (auto object : view.each())
		objects.push_back(object);

	this->gui->RenderObjectsList(objects);
}

// renders all cursors, uses common mesh
// TODO: make main cursor bigger
void Scene::cursors_system()
{
	static Mesh cursorMesh = Mesh::Cursor(0.5f);	// TODO

	this->cursorProgram->Use();
	this->cursorProgram->SetMat4("viewMatrix", camera->GetViewMatrix());
	this->cursorProgram->SetMat4("projMatrix", camera->GetProjectionMatrix());

	// torus cursors
	auto view = this->registry->view<Cursor, Transformation>();
	for (auto [entity, cursor, Transformation] : view.each())
	{
		cursorMesh.vao->Bind();
		this->cursorProgram->SetMat4("worldMatrix", Transformation.worldMatrix);

		glLineWidth(cursor.lineWidth);
		glDrawArrays(GL_LINES, 0, 6);	// TODO: move parameters to Mesh component
	}

	// main cursor
	auto [cursor, mesh, position] = this->registry->get<Cursor, Mesh, Position>(objectsManager->cursor);
	this->gui->RenderCursorWindow(objectsManager->cursor, position.position);
}

// renders points 1 by 1, uses common VAO
void Scene::points_system()
{
	this->torusProgram->Use();
	this->pointsVao->Bind();
	const unsigned int selectedCount = this->objectsManager->GetSelectedEntitiesCount();

	auto view = this->registry->view<Point, Selectable, Position, Transformation>();
	for (auto [entity, selectable, position, transf] : view.each())
	{
		this->torusProgram->SetMat4("worldMatrix", transf.worldMatrix);
		this->torusProgram->SetVec3("color", GetObjectColor(selectable.selected));
		glPointSize(5.f);
		glDrawArrays(GL_POINTS, 0, 1);

		if (selectable.selected && selectedCount == 1)
			this->gui->RenderPointGUI(selectable.name, entity, position);
	}
}
