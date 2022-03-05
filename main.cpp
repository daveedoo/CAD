#include "Window.h"
#include "Renderer.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main()
{
	Window window = Window(SCR_WIDTH, SCR_HEIGHT, "CAD");
	Renderer renderer = Renderer();

	while (!window.ShouldClose())
	{
		renderer.DrawScene();

		window.PollEvents();
		window.SwapBuffers();
	}

	return 0;
}
