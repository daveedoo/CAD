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

#include "../Window/input/events/modded/MouseClickEvent.h"

constexpr unsigned int adaptiveShadingTextureUnitIdx = 0;

Renderer::Renderer(std::shared_ptr<Window> window) : window(window)
{
	this->window->MakeContextCurrent();

	if (glewInit() != GLEW_OK)
		throw std::exception("glewInit failed");

	this->window->GetFramebufferSize(this->framebufferWidth, this->framebufferHeight);
	this->window->SetInputEventHandler([&](const InputEvent& inputEvent)
		{
			if (!ImGui::GetIO().WantCaptureMouse)
				this->scene->HandleEvent(inputEvent);
		});
	this->window->SetFramebufferSizeEventHandler([&](const ResizeEvent& ev)
		{
			this->framebufferWidth = ev.width;
			this->framebufferHeight= ev.height;

			this->UpdateMaxAdaptiveLvl();
			this->UpdateSceneFramebufferSize();
		});

	this->scene = std::make_unique<Scene>(this->framebufferWidth, this->framebufferHeight, this->window);

	this->SetupGUI();
	this->SetupAdaptiveShading();
}

void Renderer::SetupGUI()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplGlfw_InitForOpenGL(this->window->GetRawWindow(), false);
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::RenderGUI()
{
	static const float GUI_WIDTH = 400.f;
	ImGui::ShowDemoWindow();
}
