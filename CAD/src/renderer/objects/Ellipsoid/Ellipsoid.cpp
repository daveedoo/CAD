#include "Ellipsoid.h"
#include "../../../Config.h"
#include <glm/glm.hpp>

Ellipsoid::Ellipsoid(float r_x, float r_y, float r_z) :
	R_X(r_x), R_Y(r_y), R_Z(r_z)
{
	this->shininess = 1.f;

	// shaders
	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "ellipsoid.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "ellipsoid.frag");
	this->program = std::make_unique<GL::Program>(vertexShader, fragmentShader);
	this->program->SetVec4("color", this->color);
	this->program->SetFloat("shininess", this->shininess);
	this->SetFramebufferSize(SCR_WIDTH, SCR_HEIGHT);

	// VAO
	GLfloat vertices[]
	{
		-1.0f, -1.0f,		1.0f, 1.0f,		-1.0f, 1.0f,
		-1.0f, -1.0f,		1.0f, -1.0f,	1.0f, 1.0f
	};
	this->vbo = std::make_unique<GL::VBO>(vertices, sizeof(vertices));
	this->vao = std::make_unique<GL::VAO>();
	this->vao->DefineFloatAttribute(*this->vbo, 0, 2, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(float), 0);
}

void Ellipsoid::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->program->SetUint("frame_width", width);
	this->program->SetUint("frame_height", height);
}

void Ellipsoid::Render(const Camera& camera)
{
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 D = glm::mat4(
		glm::vec4(1.0 / (sqrt(this->R_X)), 0, 0, 0),
		glm::vec4(0, 1.0 / (sqrt(this->R_Y)), 0, 0),
		glm::vec4(0, 0, 1.0 / (sqrt(this->R_Z)), 0),
		glm::vec4(0, 0, 0, -1));
	glm::mat4 v_inv = glm::inverse(view);
	glm::mat4 matrix = glm::transpose(v_inv) * D * v_inv;

	this->program->SetMat4("matrix", matrix);
	this->program->SetFloat("shininess", this->shininess);
	this->program->Use();

	this->vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
