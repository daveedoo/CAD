#pragma once
#include <string>
#include <memory>
#include "../../gl/wrappers/VAO.h"
#include "../../gl/wrappers/EBO.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../../Matrix.h"


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
	float rotX;
	float rotY;
	float rotZ;

	ScaleRotation(glm::vec3 scale = glm::vec3(1.f), float rotX = 0.f, float rotY = 0.f, float rotZ = 0.f)
		: scale(scale), rotX(rotX), rotY(rotY), rotZ(rotZ) {}
};

struct Transformation
{
	glm::mat4x4 worldMatrix;

	Transformation(glm::mat4 matrix = glm::mat4(1.f))
		: worldMatrix(matrix) {}
};

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
