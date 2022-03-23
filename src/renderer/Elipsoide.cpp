#include "Elipsoide.h"
#include <Config.h>
#include <glm/glm.hpp>

Elipsoide::Elipsoide(float a, float b, float c) :
	A(a), B(b), C(c)
{
	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "vertex.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "fragment.frag");
	this->program = std::make_unique<GL::Program>(vertexShader, fragmentShader);
	this->program->SetVec4("color", glm::vec4(1.f, 1.f, 0.f, 1.f));
	this->SetFramebufferSize(SCR_WIDTH, SCR_HEIGHT);

	GLfloat vertices[]
	{
		-1.0f, -1.0f,		1.0f, 1.0f,		-1.0f, 1.0f,
		-1.0f, -1.0f,		1.0f, -1.0f,	1.0f, 1.0f
	};
	this->vbo = std::make_unique<GL::VBO>(vertices, sizeof(vertices));
	this->vao = std::make_unique<GL::VAO>();
	this->vao->DefineFloatAttribute(*this->vbo, 0, 2, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(float), 0);
}

void Elipsoide::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->program->SetUint("screen_w", width);
	this->program->SetUint("screen_h", height);
}

void Elipsoide::Draw(const Camera& camera)
{
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 D = glm::mat4(
		glm::vec4(this->A,	0,			0,			0),
		glm::vec4(0,		this->B,	0,			0),
		glm::vec4(0,		0,			this->C,	0),
		glm::vec4(0,		0,			0,			-1));
	glm::mat4 v_inv = glm::inverse(view);
	glm::mat4 matrix = glm::transpose(v_inv) * D * v_inv;

	this->program->SetMat4("matrix", matrix);
	this->program->Use();

	this->vao->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
