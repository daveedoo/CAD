#pragma once
#include <memory>
#include "../../../gl/wrappers/VAO.h"
#include "../../../gl/wrappers/Program.h"
#include "../../../Camera.h"
#include "../../../gl/wrappers/EBO.h"

class Torus
{
private:
	float minorR;
	float majorR;
	int minorSegments;
	int majorSegments;

	glm::vec3 color;

	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::EBO> ebo;
	std::unique_ptr<GL::Program> program;

	static glm::vec3 GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR);

public:
	Torus(float minorR, float majorR, unsigned int minorSegments, unsigned int majorSegments);

	float GetMinorR() { return this->minorR; }
	float GetMajorR() { return this->majorR; }
	int GetMinorSegments() { return this->minorSegments; }
	int GetMajorSegments() { return this->majorSegments; }
	void SetMinorR(float val);
	void SetMajorR(float val);
	void SetMinorSegments(int val);
	void SetMajorSegments(int val);

	void SetBuffers();
	void Render(const Camera& camera);
};
