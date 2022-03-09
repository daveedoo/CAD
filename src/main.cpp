#include "Window/Window.h"
#include "Renderer.h"
#include "Window/input/CameraMovementInputHandler.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 800

int main()
{
	Window window = Window(SCR_WIDTH, SCR_HEIGHT, "CAD");
	Renderer renderer = Renderer(window);

	Camera cam = Camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	auto cameraMove = std::make_unique<CameraMovementInputHandler>(cam);
	window.SubscribeInputHandler(std::move(cameraMove));

	while (!window.ShouldClose())
	{
		renderer.DrawScene(cam);

		window.PollEvents();
		window.SwapBuffers();
	}

	return 0;
}
