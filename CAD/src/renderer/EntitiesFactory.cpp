#include "EntitiesFactory.h"
#include <format>

EntitiesFactory::EntitiesFactory(std::shared_ptr<entt::registry> registry)
	: registry(registry)
{}

entt::entity EntitiesFactory::CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments, glm::vec3 position)
{
	static unsigned int counter = 0;
	std::string name = std::format("Torus {}", ++counter);

	const auto entity = registry->create();
	TorusComponent values(minorR, majorR, minorSegments, majorSegments);
	Mesh mesh = Mesh::Torus(values);

	this->registry->emplace<TorusComponent>(entity, values);
	this->registry->emplace<Selectable>(entity, name);
	this->registry->emplace<Position>(entity, position);
	this->registry->emplace<ScaleRotation>(entity);
	this->registry->emplace<Transformation>(entity);
	this->registry->emplace<Mesh>(entity, std::move(mesh));
	this->registry->emplace<Dirty>(entity);
	return entity;
}

entt::entity EntitiesFactory::CreatePoint(glm::vec3 position)
{
	static unsigned int counter = 0;
	std::string name = std::format("Point {}", ++counter);

	const auto entity = registry->create();
	this->registry->emplace<Point>(entity);
	this->registry->emplace<Selectable>(entity, name);
	this->registry->emplace<Position>(entity, position);
	//this->registry->emplace<Transformation>(entity);
	this->registry->emplace<Dirty>(entity);
	return entity;
}

entt::entity EntitiesFactory::CreatePoint(float pos_x, float pos_y, float pos_z)
{
	return CreatePoint(glm::vec3(pos_x, pos_y, pos_z));
}

entt::entity EntitiesFactory::CreateCursor(glm::vec3 position, GLfloat lineWidth, float lineLength)
{
	auto mesh = Mesh::Cursor(lineLength);

	const auto entity = registry->create();
	this->registry->emplace<Cursor>(entity, lineWidth, lineLength);
	this->registry->emplace<Mesh>(entity, std::move(mesh));
	this->registry->emplace<Position>(entity, position);
	//this->registry->emplace<Transformation>(entity);
	this->registry->emplace<Dirty>(entity);
	return entity;
}

entt::entity EntitiesFactory::CreateBezierC0(const std::vector<entt::entity>& points)
{
	static unsigned int counter = 0;
	std::string name = std::format("BezierC0 {}", ++counter);

	const auto& entity = registry->create();
	this->registry->emplace<Selectable>(entity, name);
	this->registry->emplace<BezierC0>(entity, points);

	return entt::entity();
}