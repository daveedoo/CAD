#include "ObjectsManager.h"
#include <format>

ObjectsManager::ObjectsManager(std::shared_ptr<entt::registry> registry)
	: registry(registry)
{
	this->cursor = CreateCursor(glm::vec3(0.f), 3.f, 1.f);

	auto point = glm::vec3(0.f);
	this->pointsVBO = std::make_unique<GL::VBO>(&point, sizeof(point));
}

entt::entity ObjectsManager::AddTorus()
{
	auto& cursorPos = this->registry->get<Translation>(this->cursor).translation;
	return CreateTorus(1.f, 5.f, 10, 10, cursorPos);
}

entt::entity ObjectsManager::AddPoint()
{
	auto& cursorPos = this->registry->get<Translation>(this->cursor).translation;
	return CreatePoint(cursorPos);
}

entt::entity ObjectsManager::CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments, glm::vec3 position)
{
	static unsigned int counter = 0;
	std::string name = std::format("Torus {}", ++counter);

	const auto entity = registry->create();
	TorusComponent values(minorR, majorR, minorSegments, majorSegments);
	Mesh mesh = Mesh::Torus(values);

	this->registry->emplace<TorusComponent>(entity, values);
	this->registry->emplace<Mesh>(entity, std::move(mesh));
	this->registry->emplace<Selectable>(entity, name);
	this->registry->emplace<Transformation>(entity, position);
	return entity;
}

entt::entity ObjectsManager::CreatePoint(glm::vec3 position)
{
	static unsigned int counter = 0;
	std::string name = std::format("Point {}", ++counter);

	const auto entity = registry->create();
	this->registry->emplace<Point>(entity, position);
	this->registry->emplace<Selectable>(entity, name);

	return entity;
}

entt::entity ObjectsManager::CreateCursor(glm::vec3 position, GLfloat lineWidth, float lineLength)
{
	auto mesh = Mesh::Cursor(lineLength);

	const auto entity = registry->create();
	this->registry->emplace<Cursor>(entity, lineWidth, lineLength);
	this->registry->emplace<Mesh>(entity, std::move(mesh));
	this->registry->emplace<Translation>(entity);

	return entity;
}

void ObjectsManager::UpdateTorusMesh(entt::entity torusEntity)
{
	auto torusComp = registry->get<TorusComponent>(torusEntity);
	auto newMesh = Mesh::Torus(torusComp);
	registry->replace<Mesh>(torusEntity, std::move(newMesh));
}

void ObjectsManager::OnObjectSelected(entt::entity entity)
{
	this->registry->get_or_emplace<Cursor>(entity, SelectedObjectCursor_LineWidth, SelectedObjectCursor_LineHeight);
}

void ObjectsManager::OnObjectUnselected(entt::entity entity)
{
	this->registry->remove<Cursor>(entity);
}

void ObjectsManager::UnselectAllObjectsExcept(entt::entity entity)
{
	auto view = this->registry->view<Selectable>();
	for (auto [entt, selectable] : view.each())
	{
		if (entt == entity)
		{
			selectable.selected = true;
			OnObjectSelected(entity);
		}
		else
		{
			selectable.selected = false;
			OnObjectUnselected(entt);
		}
	}
}

void ObjectsManager::RemoveEntity(entt::entity entity)
{
	this->registry->destroy(entity);
}
