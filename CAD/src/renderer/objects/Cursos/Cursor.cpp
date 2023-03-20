#include "Cursor.h"
#include "../../../Config.h"

std::array<glm::vec3, 12> Cursor::GetVerticesData()
{
	return {
		position,									glm::vec3(1.f, 0.f, 0.f),
		position + glm::vec3(lineLength, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f),
		position,									glm::vec3(0.f, 1.f, 0.f),
		position + glm::vec3(0.f, lineLength, 0.f), glm::vec3(0.f, 1.f, 0.f),
		position,									glm::vec3(0.f, 0.f, 1.f),
		position + glm::vec3(0.f, 0.f, lineLength), glm::vec3(0.f, 0.f, 1.f),
	};
}

Cursor::Cursor(glm::vec3 position, float lineLength, GLfloat lineWidth) :
	position(position), lineLength(lineLength), lineWidth(lineWidth)
{
	// shaders
	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "coloredVertices.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "coloredVertices.frag");
	this->program = std::make_unique<GL::Program>(vertexShader, fragmentShader);

	auto vertices = GetVerticesData();

	this->vao = std::make_unique<GL::VAO>();
	this->vbo = std::make_unique<GL::VBO>(vertices.data(), sizeof(vertices));
	this->vao->DefineFloatAttribute(*this->vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(glm::vec3), 0);
	this->vao->DefineFloatAttribute(*this->vbo, 1, 3, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(glm::vec3), sizeof(glm::vec3));

}

void Cursor::SetPosition(glm::vec3 newPosition)
{
	this->position = newPosition;

	auto newVertices = GetVerticesData();
	this->vbo->SetBufferData(newVertices.data());
}

void Cursor::Render(const Camera& camera)
{
	this->vao->Bind();
	this->program->Use();
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());

	glLineWidth(this->lineWidth);
	glDrawArrays(GL_LINES, 0, 6);
}
