#pragma once
#include <memory>
#include "../../../gl/wrappers/VAO.h"
#include "../../../gl/wrappers/Program.h"

class Torus
{
private:
	float minorR;
	float majorR;
	unsigned int minorSegments;
	unsigned int majorSegments;

	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::Program> program;

	static glm::vec3 GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR);

public:
	Torus() :Torus(0.25f, 1.f, 25, 25) {}
	Torus(float minorR, float majorR, unsigned int minorSegments, unsigned int majorSegments);
};
