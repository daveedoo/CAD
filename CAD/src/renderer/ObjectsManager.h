#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <gl/glew.h>

class ObjectsManager
{
private:
	const GLfloat SelectedObjectCursor_LineWidth = 2.f;
	const float SelectedObjectCursor_LineHeight = 0.5f;

	std::shared_ptr<entt::registry> registry;
	entt::entity cursor;

public:
	ObjectsManager(std::shared_ptr<entt::registry> registry);

	void AddTorus();

	void CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments, glm::vec3 position);
	entt::entity CreateCursor(glm::vec3 position, GLfloat lineWidth, float lineLength);
	void UpdateTorusMesh(entt::entity torusEntity);

	void OnObjectSelected(entt::entity entity);
	void OnObjectUnselected(entt::entity entity);
	void UnselectAllObjectsExcept(entt::entity entity);
	void RemoveEntity(entt::entity entity);
};
