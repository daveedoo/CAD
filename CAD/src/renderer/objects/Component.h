#pragma once
#include <string>
#include <memory>
#include "../../gl/wrappers/VAO.h"
#include "../../gl/wrappers/EBO.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <entt/entt.hpp>


struct Selectable
{
	std::string name;
	bool selected;

	Selectable(std::string name, bool selected = false) 
		: name(name), selected(selected) {}
};

struct ScaleRotation
{
	glm::vec3 scale;
	float axisFi;
	float axisLambda;
	float angle;

	ScaleRotation(glm::vec3 scale = glm::vec3(1.f), float axisFi = 0.f, float axisLambda = 0.f, float angle = 0.f)
		: scale(scale), axisFi(axisFi), axisLambda(axisLambda), angle(angle) {}
};

struct Transformation
{
	glm::mat4x4 worldMatrix;

	Transformation(glm::mat4 matrix = glm::mat4(1.f))
		: worldMatrix(matrix) {}
};

struct Dirty
{};

struct AdditionalTransformation
{
	glm::vec3 centerPoint;
	ScaleRotation scaleRotation;

	AdditionalTransformation(glm::vec3 centerPoint, ScaleRotation sr = ScaleRotation())
		: centerPoint(centerPoint), scaleRotation(sr) {}
};

struct Position
{
	glm::vec3 position;

	Position(glm::vec3 val = glm::vec3(0.f))
		: position(val) {}
};

struct ScreenPosition
{
	glm::vec3 position;

	ScreenPosition(const glm::vec3& position = glm::vec3(0.f))
		: position(position)
	{
	}
};

struct Cursor
{
	GLfloat lineWidth;
	float lineLength;

	Cursor(GLfloat lineWidth = 2.f, float lineLength = 3.f)
		: lineWidth(lineWidth), lineLength(lineLength) {}
};

struct TorusComponent
{
	float minorR;
	float majorR;
	int minorSegments;
	int majorSegments;

	TorusComponent(float minorR, float majorR, int minorSegments, int majorSegments)
		: minorR(minorR), majorR(majorR), minorSegments(minorSegments), majorSegments(majorSegments) {}
};

struct Mesh
{
	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::EBO> ebo;

	Mesh(std::unique_ptr<GL::VAO> vao, std::unique_ptr<GL::VBO> vbo, std::unique_ptr<GL::EBO> ebo = nullptr)
		: vao(std::move(vao)), vbo(std::move(vbo)), ebo(std::move(ebo)) {}

public:
	static Mesh Torus(TorusComponent val);
	static Mesh Cursor(float lineLength);

private:
	static glm::vec3 GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR);
};

struct Point
{
};

struct BezierC0
{
	std::vector<entt::entity> points;

	BezierC0(const std::vector<entt::entity>& points)
		: points(points) {}
};
