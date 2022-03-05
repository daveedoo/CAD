cmake_minimum_required(VERSION 3.8)

project("CAD")

add_executable(main "main.cpp"  "Window.h" "Window.cpp" "Config.h" "Renderer.h" "Renderer.cpp")

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE GLEW::GLEW)
