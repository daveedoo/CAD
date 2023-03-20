#include "Torus.h"
#include <glm/gtx/rotate_vector.hpp>
#include "../../../Config.h"

glm::vec3 Torus::GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR)
{
	glm::vec3 minorCirclePt = minorR * glm::vec3(glm::cos(minorAngleRad), glm::sin(minorAngleRad), 0);
	glm::vec3 majorCirclePt = majorR * glm::vec3(glm::cos(majorAngleRad), 0, glm::sin(majorAngleRad));

	return majorCirclePt + glm::rotate(minorCirclePt, majorAngleRad, glm::vec3(0.f, -1.f, 0.f));
}

Torus::Torus(float minorR, float majorR, unsigned int minorSegments, unsigned int majorSegments)
	: minorR(minorR), majorR(majorR), minorSegments(minorSegments), majorSegments(majorSegments),
	color(1.f, 1.f, 1.f)
{
	// shaders
	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "torus.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "torus.frag");
	this->program = std::make_unique<GL::Program>(vertexShader, fragmentShader);
	this->program->SetVec3("color", this->color);

	this->SetBuffers();
}

void Torus::SetMinorR(float val)
{
	this->minorR = val;
	this->SetBuffers();
}

void Torus::SetMajorR(float val)
{
	this->majorR = val;
	this->SetBuffers();
}

void Torus::SetMinorSegments(int val)
{
	this->minorSegments = val;
	this->SetBuffers();
}

void Torus::SetMajorSegments(int val)
{
	this->majorSegments = val;
	this->SetBuffers();
}

void Torus::SetBuffers()
{
	float minorStep = glm::radians(360.0 / minorSegments);
	float majorStep = glm::radians(360.0 / majorSegments);

	const unsigned int vertsCount = minorSegments * majorSegments;
	std::vector<glm::vec3> vertices(vertsCount);
	std::vector<GLuint> indices(4 * vertsCount);
	for (size_t i = 0; i < majorSegments; i++)
	{
		for (size_t j = 0; j < minorSegments; j++)
		{
			vertices[(i * minorSegments + j)] = Torus::GetTorusPoint(i * majorStep, j * minorStep, this->majorR, this->minorR);

			// minor segment line
			indices[(i * minorSegments + j) * 4 + 0] = i * minorSegments + j;
			indices[(i * minorSegments + j) * 4 + 1] = i * minorSegments + ((j + 1) % minorSegments);
			// major segment line
			indices[(i * minorSegments + j) * 4 + 2] = i * minorSegments + j;
			indices[(i * minorSegments + j) * 4 + 3] = ((i + 1) % majorSegments) * minorSegments + j;
		}
	}
	this->vao = std::make_unique<GL::VAO>();
	this->ebo = std::make_unique<GL::EBO>();
	this->ebo->SetBufferData(indices.data(), GL::EBO::DataType::UINT, 4 * vertsCount);
	this->vbo = std::make_unique<GL::VBO>(vertices.data(), sizeof(glm::vec3) * vertsCount);	// TODO: resize buffer if is already used
	this->vao->DefineFloatAttribute(*this->vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);
}

void Torus::Render(const Camera& camera)
{
	this->vao->Bind();
	this->program->Use();
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());

	glLineWidth(1.f);
	glDrawElements(GL_LINES, 4 * minorSegments * majorSegments, static_cast<GLenum>(this->ebo->GetDataType()), (void*)0);
}
