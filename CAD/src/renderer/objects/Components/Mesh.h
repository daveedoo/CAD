#pragma once
#include <memory>
#include "../../../gl/wrappers/VAO.h"
#include "../../../gl/wrappers/VBO.h"
#include "../../../gl/wrappers/EBO.h"
#include "TorusComponent.h"
#include <glm/glm.hpp>

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

