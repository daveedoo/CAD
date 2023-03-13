#include "Torus.h"
#include <glm/gtx/rotate_vector.hpp>

glm::vec3 Torus::GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR)
{
	glm::vec3 minorCirclePt = minorR * glm::vec3(glm::cos(minorAngleRad), glm::sin(minorAngleRad), 0);
	glm::vec3 majorCirclePt = majorR * glm::vec3(glm::cos(majorAngleRad), 0, glm::sin(majorAngleRad));

	return majorCirclePt + glm::rotate(minorCirclePt, majorAngleRad, glm::vec3(0.f, 1.f, 0.f));	// TODO: check for signs
}

Torus::Torus(float minorR, float majorR, unsigned int minorSegments, unsigned int majorSegments)
	: minorR(minorR), majorR(majorR), minorSegments(minorSegments), majorSegments(majorSegments)
{
	double minorStep = glm::radians(360.0 / (double)minorSegments);
	double majorStep = glm::radians(360.0 / (double)majorSegments);

	GLfloat* vertices = new GLfloat[3 * minorSegments * majorSegments];	// TODO: change to glm::vec3
	for (size_t i = 0; i < minorSegments; i++)
	{
		for (size_t j = 0; j < majorSegments; j++)
		{
			glm::vec3 point = Torus::GetTorusPoint(j * majorStep, i * minorStep, this->majorR, this->minorR);

			vertices[(i * majorSegments + j) * 3 + 0] = point.x;
			vertices[(i * majorSegments + j) * 3 + 1] = point.y;
			vertices[(i * majorSegments + j) * 3 + 2] = point.z;
		}
	}

	this->vbo = std::make_unique<GL::VBO>(vertices, sizeof(GLfloat) * minorSegments * majorSegments);
	delete[] vertices;

	this->vao = std::make_unique<GL::VAO>();
}
