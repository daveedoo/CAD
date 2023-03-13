#pragma once
#include <memory>
#include "../../../gl/wrappers/Program.h"
#include "../../../gl/wrappers/VBO.h"
#include "../../../gl/wrappers/VAO.h"
#include "../../../Camera.h"
#include "../../../gl/wrappers/FBO.h"

class Ellipsoid
{
public:
	float R_X, R_Y, R_Z;
	float shininess;
	const glm::vec4 color = glm::vec4(1.f, 1.f, 0.f, 1.f);

	/// <summary>
	/// Creates a sphere with radius = 1
	/// </summary>
	Ellipsoid() : Ellipsoid(1.f, 1.f, 1.f) {}
	Ellipsoid(float r_x, float r_y, float r_z);

	void Render(const Camera& camera);
	void SetFramebufferSize(unsigned int width, unsigned int height);	// TODO: remove it?

private:
	std::unique_ptr<GL::Program> program;
	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
};
