#pragma once
#include <string>
#include <memory>
#include "../../gl/wrappers/VAO.h"
#include "../../gl/wrappers/EBO.h"
#include <glm/glm.hpp>


struct Selectable
{
	std::string name;
	bool selected;

	Selectable(std::string name, bool selected = false) 
		: name(name), selected(selected) {}
};

struct Transformation
{
	glm::vec3 translation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);
	float rotX = 0.f;
	float rotY = 0.f;
	float rotZ = 0.f;

	glm::mat4x4 worldMatrix = glm::mat4(1.f);

	Transformation() = default;
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

	Mesh(std::unique_ptr<GL::VAO> vao, std::unique_ptr<GL::VBO> vbo, std::unique_ptr<GL::EBO> ebo)
		: vao(std::move(vao)), vbo(std::move(vbo)), ebo(std::move(ebo)) {}

public:
	static Mesh Torus(TorusComponent val);

private:
	static glm::vec3 GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR);
};
