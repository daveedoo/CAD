#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "objects/Component.h"
#include <optional>

class ObjectsManager
{
private:
	const GLfloat SelectedObjectCursor_LineWidth = 2.f;
	const float SelectedObjectCursor_LineLength = 0.5f;

	std::shared_ptr<entt::registry> registry;
	std::vector<glm::vec3> points;

public:
	std::unique_ptr<GL::VBO> pointsVBO;
	entt::entity cursor;
	entt::entity selectedEnttsCursor;

	ObjectsManager(std::shared_ptr<entt::registry> registry);
	unsigned int GetSelectedEntitiesCount(glm::vec3* meanCursorPos = nullptr);

	entt::entity AddTorus();
	entt::entity AddPoint();
	entt::entity CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments, glm::vec3 position);
	entt::entity CreatePoint(glm::vec3 position);
	entt::entity CreateCursor(glm::vec3 position, GLfloat lineWidth, float lineLength);
	void UpdateTorusMesh(entt::entity torusEntity);
	void UpdateTransformation(entt::entity entity);

	void RecalculateMeanCursor();
	void OnObjectSelected(entt::entity entity);
	void OnObjectUnselected(entt::entity entity);
	void UnselectAllObjectsExcept(entt::entity entity);
	void RemoveEntity(entt::entity entity);

	void StartGroupTransformations();
	ScaleRotation& GetGroupTransformations();
	void SetGroupTransformations(ScaleRotation scaleRotation);
	void EndGroupTransformations(bool apply);
};
