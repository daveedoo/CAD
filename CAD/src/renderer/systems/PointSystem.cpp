#include "PointSystem.h"
#include "../../gl/ProgramFactory.h"
#include <glm\gtc\type_ptr.hpp>
#include "..\objects\Components\Point.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\..\Utils.h"

PointSystem::PointSystem(std::shared_ptr<entt::registry> registry) : System(registry),
	vao(std::make_unique<GL::VAO>()),
	vbo(std::make_unique<GL::VBO>(glm::value_ptr(glm::vec3(0.f)), sizeof(glm::vec3))),
	program(ProgramFactory::CreateProgram("torus.vert", "torus.frag"))
{
	this->vao->DefineFloatAttribute(*this->vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);
}

void PointSystem::Update(const Camera& camera)
{
}

void PointSystem::Render(const Camera& camera)
{
	this->program->Use();
	this->vao->Bind();
	
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());
	glPointSize(5.f);

	auto view = this->registry->view<Point, Position, Transformation>();
	for (auto [entity, position, transf] : view.each())
	{
		auto selectable = this->registry->try_get<Selectable>(entity);
		glm::vec3 color = selectable == nullptr ? glm::vec3(0.7f) : Utils::GetObjectColor(selectable->selected);

		this->program->SetMat4("worldMatrix", transf.worldMatrix);
		this->program->SetVec3("color", color);
		glDrawArrays(GL_POINTS, 0, 1);
	}
}
