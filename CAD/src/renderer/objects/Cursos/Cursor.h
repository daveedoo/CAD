#pragma once
#include <memory>
#include "../../../gl/wrappers/VAO.h"
#include "../../../gl/wrappers/Program.h"
#include "../../../Camera.h"
#include <array>

class Cursor
{
private:
	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::Program> program;

	glm::vec3 position;
	GLfloat lineWidth;
	float lineLength;

	std::array<glm::vec3, 12> GetVerticesData();

public:
	Cursor(glm::vec3 position = glm::vec3(0.f), float lineLength = 0.5f, GLfloat lineWidth = 3.f);

	glm::vec3 GetPosition() { return this->position; }
	void SetPosition(glm::vec3 newPosition);

	void Render(const Camera& camera);
};
