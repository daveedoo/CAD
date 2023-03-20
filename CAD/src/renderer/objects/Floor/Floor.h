#pragma once
#include <memory>
#include "../../../gl/wrappers/VAO.h"
#include "../../../gl/wrappers/Program.h"
#include "../../../Camera.h"
#include "../../../gl/wrappers/EBO.h"


class Floor
{
private:
	int xUnits;
	int zUnits;

	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::EBO> ebo;
	std::unique_ptr<GL::Program> program;

public:
	Floor(int xUnits, int zUnits);
	void Render(const Camera& camera) const;
};