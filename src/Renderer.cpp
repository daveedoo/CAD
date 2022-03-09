#include <gl/glew.h>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>

#include <Config.h>
#include "Renderer.h"
#include "gl/wrappers/VAO.h"
#include "gl/wrappers/Shader.h"
#include "gl/wrappers/Program.h"

Renderer::Renderer(const Window& window) : window(window)
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::exception("glewInit failed");

	glClearColor(1.0f, 0.66f, 0.4f, 1.0f);
}

void Renderer::DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat vertices[]
	{
		-1.0f, -1.0f,		1.0f, 1.0f,		-1.0f, 1.0f,
		-1.0f, -1.0f,		1.0f, -1.0f,	1.0f, 1.0f
	};

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	float A = 5.0;
	float B = 10.0;
	float C = 5.0;
	glm::mat4 D = glm::mat4(glm::vec4(A, 0, 0, 0),
					glm::vec4(0, B, 0, 0),
					glm::vec4(0, 0, C, 0),
					glm::vec4(0, 0, 0, -1));
	//glm::mat4 matrix = glm::transpose(glm::inverse(view)) * D * glm::inverse(view);
	glm::mat4 matrix = D;

	auto shadersPath = std::filesystem::path(SHADERS_DIR);
	GL::Shader vertexShader(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "vertex.vert");
	GL::Shader fragmentShader(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "fragment.frag");

	GL::Program program(vertexShader, fragmentShader);
	program.SetMat4("matrix", matrix);
	program.Use();

	GL::VAO vao;
	GL::VBO vbo(vertices, sizeof(vertices));
	vao.DefineFloatAttribute(vbo, 0, 2, GL::VAO::FloatAttribute::FLOAT, 2 * sizeof(float), 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);




	//float A = 5.0;
	//float B = 5.0;
	//float C = 1.0;
	//glm::mat4 D = glm::mat4(glm::vec4(A, 0, 0, 0),
	//	glm::vec4(0, B, 0, 0),
	//	glm::vec4(0, 0, C, 0),
	//	glm::vec4(0, 0, 0, -1));

	//float x = 0.5;
	//float y = 0.5;

	//float a = D[2][2];
	//float b = x * D[2][0] + y * D[2][1] + x * D[0][2] + y * D[1][2] + D[3][2] + D[2][3];
	//float c = x * (x * D[0][0] + y * D[1][0] + D[3][0] + D[0][3])
	//	+ y * (x * D[0][1] + y * D[1][1] + D[3][1] + D[1][3])
	//	+ D[3][3];
	//float delta = b * b - 4 * a * c;


	////glm::mat4 model = glm::mat4(1.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	////glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)800 / (float)600, 0.1f, 100.f);	// TODO: parameterize
	//glm::mat4 proj = glm::ortho(-10.f, 10.f, -10.f, 10.f, 10.f, -10.f);

	//float a = 1.f, b = 1.f, c = 1.f;
	//glm::mat4 D = glm::mat4();// a, b, c, -1.f);
	//D[0][0] = a;
	//D[1][1] = b;
	//D[2][2] = c;
	//D[3][3] = -1.f;

	//glm::mat4 PV_inv = glm::inverse(proj * view);
	//glm::mat4 D_M = glm::transpose(PV_inv) * D * PV_inv;
}
