#include <gl/glew.h>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "../Config.h"
#include "../gl/wrappers/VAO.h"
#include "../gl/wrappers/Shader.h"
#include "../gl/wrappers/Program.h"
#include "../Window/Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include "../Window/input/events/modded/MouseClickEvent.h"

constexpr unsigned int adaptiveShadingTextureUnitIdx = 0;

Renderer::Renderer(Window& window) : window(window)
{
	this->window.MakeContextCurrent();

	if (glewInit() != GLEW_OK)
		throw std::exception("glewInit failed");

	this->window.GetFramebufferSize(this->framebufferWidth, this->framebufferHeight);
	this->window.SetInputEventHandler([&](const InputEvent& inputEvent)
		{
			if (!ImGui::GetIO().WantCaptureMouse)
				this->scene->HandleEvent(inputEvent);
		});
	this->window.SetFramebufferSizeEventHandler([&](const ResizeEvent& ev)
		{
			this->framebufferWidth = ev.width;
			this->framebufferHeight= ev.height;

			this->UpdateMaxAdaptiveLvl();
			this->UpdateSceneFramebufferSize();
		});

	this->scene = std::make_unique<Scene>(this->framebufferWidth, this->framebufferHeight);

	this->SetupGUI();
	this->SetupAdaptiveShading();
}

void Renderer::SetupGUI()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window.GetRawWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void Renderer::SetupAdaptiveShading()
{
	auto shadersPath = std::filesystem::path(SHADERS_DIR);	// TODO: pass it as parameter
	GL::Shader adaptiveVS(GL::Shader::ShaderType::VERTEX_SHADER, shadersPath / "texture.vert");
	GL::Shader adaptiveFS(GL::Shader::ShaderType::FRAGMENT_SHADER, shadersPath / "texture.frag");
	this->adaptiveShadingProgram = std::make_unique<GL::Program>(adaptiveVS, adaptiveFS);

	this->adaptiveShadingTexture = std::make_shared<GL::Texture2D>(this->framebufferWidth / this->adaptiveLvl, this->framebufferHeight / this->adaptiveLvl,
		GL::Texture::InternalFormat::RGB_32F,
		GL::Texture::MinFilter::NEAREST,
		GL::Texture::MagFilter::NEAREST);
	this->adaptiveShadingFBO = std::make_unique<GL::FBO>(this->framebufferWidth, this->framebufferHeight);
	this->adaptiveShadingFBO->SetColorAttachment(adaptiveShadingTextureUnitIdx, adaptiveShadingTexture);
	this->adaptiveShadingTexture->Bind(adaptiveShadingTextureUnitIdx);
	this->adaptiveShadingProgram->SetInt("tex", adaptiveShadingTextureUnitIdx);

	float textureQuadData[] =
	{
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f
	};
	this->adaptiveShadingVBO = std::make_unique<GL::VBO>(
		reinterpret_cast<const uint8_t*>(textureQuadData),
		sizeof(textureQuadData));

	this->adaptiveShadingVAO = std::make_unique<GL::VAO>();
	this->adaptiveShadingVAO->DefineFloatAttribute(*this->adaptiveShadingVBO, 0,
		2,
		GL::VAO::FloatAttribute::FLOAT,
		4 * sizeof(float),
		0);
	this->adaptiveShadingVAO->DefineFloatAttribute(*this->adaptiveShadingVBO, 1,
		2,
		GL::VAO::FloatAttribute::FLOAT,
		4 * sizeof(float),
		2 * sizeof(float));

	this->UpdateMaxAdaptiveLvl();
	this->UpdateSceneFramebufferSize();
}


Renderer::~Renderer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Renderer::Update()
{
	this->scene->Update();
}

void Renderer::UpdateMaxAdaptiveLvl()
{
	int maxAdaptiveLvl = glm::min(this->framebufferWidth, this->framebufferHeight);
	if (this->adaptiveLvl > maxAdaptiveLvl)
		this->adaptiveLvl = maxAdaptiveLvl;
}

// it is called on change of:
//  - adaptiveShadingOn
//  - adaptiveLvl
//  - framebuffer size
//  - when the scene starts or stops moving
void Renderer::UpdateSceneFramebufferSize()
{
	// send info to scene class, that frame size has changed...
	int w = this->framebufferWidth;
	int h = this->framebufferHeight;
	if (this->ShouldBeRenderedAdaptively())
	{
		w /= this->adaptiveLvl;
		h /= this->adaptiveLvl;
	}
	this->scene->SetFramebufferSize(w, h);

	// and update the adaptive texture accordingly too
	if (this->ShouldBeRenderedAdaptively())
	{
		// when adaptive shading is OFF, texture is not used, so no update needed
		this->adaptiveShadingTexture->SetSize(w, h);
	}
}

bool Renderer::ShouldBeRenderedAdaptively() const
{
	return this->adaptiveShadingOn && this->scene->IsSceneMoving();
}

void Renderer::Render()
{
	if (this->ShouldBeRenderedAdaptively())
	{
		// 1st pass
		this->adaptiveShadingFBO->Bind();
		glViewport(0, 0, this->framebufferWidth / this->adaptiveLvl, this->framebufferHeight / this->adaptiveLvl);
		this->scene->Render();

		// 2nd pass
		GL::FBO::Unbind();
		this->adaptiveShadingProgram->Use();
		this->adaptiveShadingVAO->Bind();
		glViewport(0, 0, this->framebufferWidth, this->framebufferHeight);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else
	{
		GL::FBO::Unbind();
		glViewport(0, 0, this->framebufferWidth, this->framebufferHeight);
		this->scene->Render();
	}


	this->RenderGUI();
}

void Renderer::RenderGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static const float GUI_WIDTH = 400.f;

	ImVec2 nextWndPos{};
	ImVec2 wndSize{};
	//ImGui::SetNextWindowPos(nextWndPos);
	//ImGui::SetNextWindowSize(ImVec2(GUI_WIDTH, 0.f));
	//ImGui::Begin("Ellipsoid");
	//ImGui::DragFloat("X semi-axis", &this->scene->ellipsoid->R_X, 0.001f, 0.001f, 10.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	//ImGui::DragFloat("Y semi-axis", &this->scene->ellipsoid->R_Y, 0.001f, 0.001f, 10.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	//ImGui::DragFloat("Z semi-axis", &this->scene->ellipsoid->R_Z, 0.001f, 0.001f, 10.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	//nextWndPos.y += ImGui::GetWindowSize().y;
	//ImGui::End();

	//ImGui::SetNextWindowPos(ImVec2(0, nextWndPos.y));
	//ImGui::SetNextWindowSize(ImVec2(GUI_WIDTH, 0));
	//ImGui::Begin("Light");
	//ImGui::DragFloat("Shininess", &this->scene->ellipsoid->shininess, 0.1f, 0.f, 100.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	//ImVec2 wndSize = ImGui::GetWindowSize();
	//nextWndPos = ImVec2(nextWndPos.x + wndSize.x, nextWndPos.y + wndSize.y);
	//ImGui::End();

	//ImGui::SetNextWindowPos(ImVec2(0, nextWndPos.y));
	//ImGui::SetNextWindowSize(ImVec2(GUI_WIDTH, 0));
	//ImGui::Begin("Adaptive shading");
	//if (ImGui::Checkbox("ON", &this->adaptiveShadingOn))
	//{
	//	this->UpdateSceneFramebufferSize();
	//}
	//if (!this->adaptiveShadingOn)
	//	ImGui::BeginDisabled();
	//if (ImGui::DragInt("Adaptive level", &this->adaptiveLvl, 1.f, 1, glm::min(this->framebufferWidth, this->framebufferHeight), "%d", ImGuiSliderFlags_AlwaysClamp))
	//{
	//	this->UpdateMaxAdaptiveLvl();
	//	this->UpdateSceneFramebufferSize();
	//}
	//if (!this->adaptiveShadingOn)
	//	ImGui::EndDisabled();
	//wndSize = ImGui::GetWindowSize();
	//nextWndPos = ImVec2(nextWndPos.x + wndSize.x, nextWndPos.y + wndSize.y);
	//ImGui::End();

	ImGui::ShowDemoWindow();
	ImGui::SetNextWindowPos(nextWndPos);
	ImGui::SetNextWindowSize(ImVec2(GUI_WIDTH, 0));
	ImGui::Begin("Torus");
	if (ImGui::TreeNode("Parameters"))
	{
		static float minorR = this->scene->torus->GetMinorR();
		static float majorR = this->scene->torus->GetMajorR();
		static int minorSegments = this->scene->torus->GetMinorSegments();
		static int majorSegments = this->scene->torus->GetMajorSegments();
		if (ImGui::DragFloat("Minor R", &minorR, 0.001f, 0.01f, 10.f, "%3f", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetMinorR(minorR);
		if (ImGui::DragFloat("Major R", &majorR, 0.001f, 0.01f, 10.f, "%3f", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetMajorR(majorR);
		if (ImGui::DragInt("Minor segments", &minorSegments, 1, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetMinorSegments(minorSegments);
		if (ImGui::DragInt("Major segments", &majorSegments, 1, 3, 200, "%d", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetMajorSegments(majorSegments);
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Transformations", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static glm::vec3 scale(1.f, 1.f, 1.f);
		static float rotX = 0.f, rotY = 0.f, rotZ = 0.f;
		static glm::vec3 translation;
		if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f))
			this->scene->torus->SetScale(scale);

		if (ImGui::DragFloat("RotX", &rotX, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetRotationX(rotX);
		if (ImGui::DragFloat("RotY", &rotY, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetRotationY(rotY);
		if (ImGui::DragFloat("RotZ", &rotZ, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			this->scene->torus->SetRotationZ(rotZ);


		if (ImGui::DragFloat3("Translation", glm::value_ptr(translation), 0.01f))
			this->scene->torus->SetTranslation(translation);

		ImGui::TreePop();
	}
	wndSize = ImGui::GetWindowSize();
	nextWndPos = ImVec2(0.f, nextWndPos.y + wndSize.y);
	ImGui::End();

	static glm::vec3 cursorPosition = this->scene->cursor->GetPosition();
	ImGui::SetNextWindowPos(nextWndPos);
	ImGui::SetNextWindowSize(ImVec2(GUI_WIDTH, 0));
	ImGui::Begin("Cursor");
	if (ImGui::DragFloat3("Position", glm::value_ptr(cursorPosition), 0.1f, 0.f, 0.f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		this->scene->cursor->SetPosition(cursorPosition);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
