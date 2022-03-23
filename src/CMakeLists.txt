cmake_minimum_required(VERSION 3.8)

#add_subdirectory("gl")

add_executable(${PROJECT_NAME} "main.cpp" "Window/Window.h" "Window/Window.cpp" "../Config.h" "renderer/Renderer.h" "renderer/Renderer.cpp"
	"gl/wrappers/VAO.h" "gl/wrappers/VAO.cpp" "gl/wrappers/VBO.h" "gl/wrappers/VBO.cpp"
	"gl/wrappers/Shader.h" "gl/wrappers/Shader.cpp" "gl/wrappers/Program.h" "gl/wrappers/Program.cpp"
	"Camera.h" "Camera.cpp"
	"Window/input/handlers/InputHandler.h" "Window/input/handlers/CameraMovementInputHandler.h" "Window/input/handlers/CameraMovementInputHandler.cpp"
	"Window/input/events/InputEvent.h"
	"Window/input/events/modded/KeyOrButtonEvent.h" "Window/input/events/modded/KeyEvent.h"
	"Window/input/events/modded/MouseClickEvent.h" "Window/input/events/MouseMoveEvent.h"
	"Window/input/events/MouseScrollEvent.h"
	"renderer/GUI.h" "renderer/GUI.cpp" "renderer/Scene.h" "renderer/Scene.cpp" "renderer/Coefficients.h"
	"renderer/Elipsoide.h" "renderer/Elipsoide.cpp"
	"Window/input/events/ResizeEvent.h")

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
