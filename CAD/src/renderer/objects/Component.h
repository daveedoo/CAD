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

struct Transformation
{
	glm::vec3 translation;
	glm::vec3 scale = glm::vec3(1.f);
	float rotX = 0.f;
	float rotY = 0.f;
	float rotZ = 0.f;

	glm::mat4x4 worldMatrix;


	Transformation(glm::vec3 translation = glm::vec3(0.f))
		: translation(translation), worldMatrix(Matrix::Translation(translation)) {}
};

struct Translation
{
	glm::vec3 translation;

	Translation(glm::vec3 val = glm::vec3(0.f))
		: translation(val) {}
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
