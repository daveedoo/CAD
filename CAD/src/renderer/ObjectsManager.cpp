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
	auto& cursorPos = this->registry->get<Position>(this->cursor).position;
	return CreateTorus(1.f, 5.f, 10, 10, cursorPos);
}

entt::entity ObjectsManager::AddPoint()
{
	auto& cursorPos = this->registry->get<Position>(this->cursor).position;
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
	this->registry->emplace<Selectable>(entity, name);
	this->registry->emplace<Position>(entity, glm::vec3(position.x, position.y, position.z));
	this->registry->emplace<ScaleRotation>(entity);
	this->registry->emplace<Transformation>(entity);
	this->registry->emplace<Mesh>(entity, std::move(mesh));
	UpdateTransformation(entity);
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
	this->registry->emplace<Position>(entity);
	this->registry->emplace<Transformation>(entity);

	return entity;
}

void ObjectsManager::UpdateTorusMesh(entt::entity torusEntity)
{
	auto torusComp = registry->get<TorusComponent>(torusEntity);
	auto newMesh = Mesh::Torus(torusComp);
	registry->replace<Mesh>(torusEntity, std::move(newMesh));
}

void ObjectsManager::UpdateTransformation(entt::entity entity)
{
	auto [pos, sr, transf] = this->registry->try_get<Position, ScaleRotation, Transformation>(entity);
	
	transf->worldMatrix = glm::mat4(1.f);
	if (pos != nullptr)
		transf->worldMatrix *= Matrix::Translation(pos->position);
	if (sr != nullptr)
		transf->worldMatrix *=
			Matrix::RotationZ(glm::radians(sr->rotZ)) * Matrix::RotationY(glm::radians(sr->rotY)) * Matrix::RotationX(glm::radians(sr->rotX)) *
			Matrix::Scale(sr->scale);
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
