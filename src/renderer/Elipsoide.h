#pragma once
#include <memory>
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VBO.h"
#include "../gl/wrappers/VAO.h"
#include "../Camera.h"

class Elipsoide
{
private:
	float A, B, C;

	std::unique_ptr<GL::Program> program;
	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;

public:
	Elipsoide() : Elipsoide(1.f, 1.f, 1.f) {}
	Elipsoide(float a, float b, float c);

	void SetA(float a) { this->A = a; }
	void SetB(float b) { this->B = b; }
	void SetC(float c) { this->C = c; }
	void SetFramebufferSize(unsigned int width, unsigned int height);
	void Draw(const Camera& camera);
};
