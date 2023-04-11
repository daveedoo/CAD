#include "Component.h"
#include <glm/gtx/rotate_vector.hpp>
#include <memory>
#include <vector>
#include <array>

Mesh Mesh::Torus(TorusComponent val)
{
	float minorStep = glm::radians(360.0 / val.minorSegments);
	float majorStep = glm::radians(360.0 / val.majorSegments);

	const unsigned int vertsCount = val.minorSegments * val.majorSegments;
	std::vector<glm::vec3> vertices(vertsCount);
	std::vector<GLuint> indices(4 * vertsCount);
	for (size_t i = 0; i < val.majorSegments; i++)
	{
		for (size_t j = 0; j < val.minorSegments; j++)
		{
			vertices[(i * val.minorSegments + j)] = Mesh::GetTorusPoint(i * majorStep, j * minorStep, val.majorR, val.minorR);

			// minor segment line
			indices[(i * val.minorSegments + j) * 4 + 0] = i * val.minorSegments + j;
			indices[(i * val.minorSegments + j) * 4 + 1] = i * val.minorSegments + ((j + 1) % val.minorSegments);
			// major segment line
			indices[(i * val.minorSegments + j) * 4 + 2] = i * val.minorSegments + j;
			indices[(i * val.minorSegments + j) * 4 + 3] = ((i + 1) % val.majorSegments) * val.minorSegments + j;
		}
	}
	std::unique_ptr<GL::VAO> vao = std::make_unique<GL::VAO>();
	std::unique_ptr<GL::EBO> ebo = std::make_unique<GL::EBO>();
	ebo->SetBufferData(indices.data(), GL::EBO::DataType::UINT, 4 * vertsCount);
	std::unique_ptr<GL::VBO> vbo = std::make_unique<GL::VBO>(vertices.data(), sizeof(glm::vec3) * vertsCount);	// TODO: resize buffer if is already used
	vao->DefineFloatAttribute(*vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);

	return Mesh(std::move(vao), std::move(vbo), std::move(ebo));
}

Mesh Mesh::Cursor(float lineLength)
{
	std::array<glm::vec3, 12> vertices ={
		glm::vec3(0.f),										glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f)+ glm::vec3(lineLength, 0.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(0.f),										glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f)+ glm::vec3(0.f, lineLength, 0.f),	glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f),										glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.f)+ glm::vec3(0.f, 0.f, lineLength),	glm::vec3(0.f, 0.f, 1.f)
	};

	auto vao = std::make_unique<GL::VAO>();
	auto vbo = std::make_unique<GL::VBO>(vertices.data(), sizeof(vertices));
	vao->DefineFloatAttribute(*vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(glm::vec3), 0);
	vao->DefineFloatAttribute(*vbo, 1, 3, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(glm::vec3), sizeof(glm::vec3));

	return Mesh(std::move(vao), std::move(vbo));
}

glm::vec3 Mesh::GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR)
{
	glm::vec3 minorCirclePt = minorR * glm::vec3(glm::cos(minorAngleRad), glm::sin(minorAngleRad), 0);
	glm::vec3 majorCirclePt = majorR * glm::vec3(glm::cos(majorAngleRad), 0, glm::sin(majorAngleRad));

	return majorCirclePt + glm::rotate(minorCirclePt, majorAngleRad, glm::vec3(0.f, -1.f, 0.f));
}
