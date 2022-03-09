cmake_minimum_required(VERSION 3.8)

#add_subdirectory("gl")

add_executable(${PROJECT_NAME} "main.cpp" "Window/Window.h" "Window/Window.cpp" "../Config.h" "Renderer.h" "Renderer.cpp"
	"gl/wrappers/VAO.h" "gl/wrappers/VAO.cpp" "gl/wrappers/VBO.h" "gl/wrappers/VBO.cpp"
	"gl/wrappers/Shader.h" "gl/wrappers/Shader.cpp" "gl/wrappers/Program.h" "gl/wrappers/Program.cpp"
	"Window/input/InputEvent.h" "Window/input/InputHandler.h" "Window/input/CameraMovementInputHandler.h" "Window/input/CameraMovementInputHandler.cpp"
	"Window/Camera.h" "Window/Camera.cpp")

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
