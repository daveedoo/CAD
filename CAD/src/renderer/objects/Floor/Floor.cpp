#include "Floor.h"
#include "../../../Config.h"

Floor::Floor(int xUnits, int zUnits) : xUnits(xUnits), zUnits(zUnits)
{
	glm::vec3 color(0.3f, 0.3f, 0.3f);

	// shaders
	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "torus.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "torus.frag");
	this->program = std::make_unique<GL::Program>(vertexShader, fragmentShader);
	this->program->SetVec3("color", color);

	// buffers
	const unsigned int verticesCount = 4 * (xUnits + zUnits);
	const unsigned int edgesCount = 2 * (xUnits + zUnits + 1);
	std::vector<glm::vec3> vertices(verticesCount);
	std::vector<unsigned int> edges(2*edgesCount);
	size_t idx = 0;
	for (int i = -xUnits; i <= xUnits; i++)
	{
		edges[idx] = idx;
		vertices[idx++] = glm::vec3(i, 0.f, -zUnits);
		edges[idx] = idx;
		vertices[idx++] = glm::vec3(i, 0.f, zUnits);
	}
	for (int i = -zUnits + 1; i < zUnits; i++)
	{
		edges[idx] = idx;
		vertices[idx++] = glm::vec3(-xUnits, 0.f, i);
		edges[idx] = idx;
		vertices[idx++] = glm::vec3(xUnits, 0.f, i);
	}
	edges[idx++] = 0;
	edges[idx++] = 4 * xUnits;
	edges[idx++] = 1;
	edges[idx++] = 4 * xUnits + 1;
	
	this->vao = std::make_unique<GL::VAO>();
	this->vbo = std::make_unique<GL::VBO>(vertices.data(), sizeof(glm::vec3) * verticesCount);
	this->ebo = std::make_unique<GL::EBO>();
	this->ebo->SetBufferData(edges.data(), GL::EBO::DataType::UINT, 2 * edgesCount);
	this->vao->DefineFloatAttribute(*this->vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);
}

void Floor::Render(const Camera& camera) const
{
	this->vao->Bind();
	this->program->Use();
	this->program->SetMat4("worldMatrix", glm::mat4(1.f));
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());

	glLineWidth(1.f);
	glDrawElements(GL_LINES, 4 * (this->xUnits + this->zUnits + 1), static_cast<GLenum>(this->ebo->GetDataType()), static_cast<const void*>(0));
}
