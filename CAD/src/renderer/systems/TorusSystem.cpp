#include "TorusSystem.h"
#include "../../gl/ProgramFactory.h"
#include <iostream>


void on_update(entt::registry& registry, entt::entity entity)
{
	auto torusComp = registry.get<TorusComponent>(entity);
	auto newMesh = Mesh::Torus(torusComp);
	registry.replace<Mesh>(entity, std::move(newMesh));
}

TorusSystem::TorusSystem(std::shared_ptr<entt::registry> registry) : System(registry),
	torusProgram(ProgramFactory::CreateProgram("torus.vert", "torus.frag"))
{
	this->torusProgram->SetVec3("color", glm::vec3(1.f));

	this->registry->on_update<TorusComponent>().connect<on_update>();
}

glm::vec3 TorusSystem::GetObjectColor(bool isSelected)
{
	return isSelected ? glm::vec3(1.f, 0.65f, 0.f) : glm::vec3(1.f, 1.f, 1.f);
}

void TorusSystem::Update(const Camera& camera)
{
}

void TorusSystem::Render(const Camera& camera)
{
	this->torusProgram->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->torusProgram->SetMat4("projMatrix", camera.GetProjectionMatrix());
	glLineWidth(1.f);

	auto view = this->registry->view<TorusComponent, Mesh, Selectable, Position, ScaleRotation, Transformation>();
	for (auto [entt, torusComp, mesh, selectable, position, sr, transf] : view.each())
	{
		glm::mat4 worldMtx = transf.worldMatrix;

		mesh.vao->Bind();
		this->torusProgram->Use();
		this->torusProgram->SetMat4("worldMatrix", worldMtx);
		this->torusProgram->SetVec3("color", GetObjectColor(selectable.selected));
		glDrawElements(GL_LINES, 4 * torusComp.minorSegments * torusComp.majorSegments, static_cast<GLenum>(mesh.ebo->GetDataType()), static_cast<void*>(0));
	}
}
