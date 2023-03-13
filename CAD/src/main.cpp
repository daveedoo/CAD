#include <gl/glew.h>
#include "Config.h"
#include "Window/Window.h"
#include "renderer/Renderer.h"
#include "Window/input/handlers/CameraMovementInputHandler.h"

int main()
{
	Window window = Window(SCR_WIDTH, SCR_HEIGHT, "CAD");
	Renderer renderer = Renderer(window);

	while (!window.ShouldClose())
	{
		renderer.Update();
		renderer.Render();

		window.SwapBuffers();
		window.PollEvents();
	}

	return 0;
}
