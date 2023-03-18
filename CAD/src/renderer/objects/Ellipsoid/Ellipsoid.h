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
	Ellipsoid(unsigned int frame_width, unsigned int frame_height) : Ellipsoid(1.f, 1.f, 1.f, frame_width, frame_height) {}
	/// <param name="Rs">Vector of semi-axes lengths</param>
	Ellipsoid(glm::vec3 Rs, unsigned int frame_width, unsigned int frame_height) : Ellipsoid(Rs.x, Rs.y, Rs.z, frame_width, frame_height) {}
	Ellipsoid(float r_x, float r_y, float r_z, unsigned int frame_width, unsigned int frame_height);

	void Render(const Camera& camera);
	void SetFramebufferSize(unsigned int width, unsigned int height);

private:
	std::unique_ptr<GL::Program> program;
	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
};
