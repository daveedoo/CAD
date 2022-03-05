#include "Renderer.h"
#include <gl/glew.h>
#include <exception>

Renderer::Renderer()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::exception("glewInit failed");

	glClearColor(1.0f, 0.66f, 0.4f, 1.0f);
}

void Renderer::DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
}