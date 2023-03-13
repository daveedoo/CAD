//#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//
//GUI::GUI(Window& window) : window(window)
//{
//	IMGUI_CHECKVERSION();	// TODO: ??
//
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//
//	ImGui_ImplGlfw_InitForOpenGL(this->window.GetRawWindow(), true);
//	ImGui_ImplOpenGL3_Init(NULL);
//}
//
//GUI::~GUI()
//{
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//}
//
//bool GUI::WantCaptureMouse()
//{
//	ImGuiIO& io = ImGui::GetIO();
//	return io.WantCaptureMouse;
//}
//
//void GUI::Draw()
//{
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplGlfw_NewFrame();
//	ImGui::NewFrame();
//
//	//ImGui::ShowDemoWindow();
//
//	float width = 300.f;
//	ImGui::SetNextWindowPos(ImVec2(0, 0));
//	ImGui::SetNextWindowSize(ImVec2(width, 0.f));
//	ImGui::Begin("Elipsoid");
//	ImGui::DragFloat("A", &this->coefficients.A, 0.01f, 0.0f, 10.0f, "%.2f");
//	ImGui::DragFloat("B", &this->coefficients.B, 0.01f, 0.0f, 10.0f, "%.2f");
//	ImGui::DragFloat("C", &this->coefficients.C, 0.01f, 0.0f, 10.0f, "%.2f");
//	ImVec2 size = ImGui::GetWindowSize();
//	ImGui::End();
//
//	ImGui::SetNextWindowPos(ImVec2(0, size.y));
//	ImGui::SetNextWindowSize(ImVec2(width, 0));
//	ImGui::Begin("Light");
//	//ImGui::DragFloat("shininess", &this->coefficients.A, 0.1f, 0.f, 1.f);
//	//size = ImGui::GetWindowSize();
//	ImGui::End();
//
//	ImGui::Render();
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}
