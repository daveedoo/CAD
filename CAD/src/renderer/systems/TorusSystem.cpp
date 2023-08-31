#include "TorusSystem.h"
#include "../../gl/ProgramFactory.h"
#include "..\objects\Components\TorusComponent.h"
#include "..\objects\Components\Mesh.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Rotation.h"
#include "..\objects\Components\Scaling.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\..\Utils.h"


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

void TorusSystem::Update(const Camera& camera)
{
}

void TorusSystem::Render(const Camera& camera)
{
	this->torusProgram->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->torusProgram->SetMat4("projMatrix", camera.GetProjectionMatrix());
	glLineWidth(1.f);

	auto view = this->registry->view<TorusComponent, Mesh, Selectable, Position, Scaling, Rotation, Transformation>();
	for (auto [entt, torusComp, mesh, selectable, position, scaling, rotation, transf] : view.each())
	{
		glm::mat4 worldMtx = transf.worldMatrix;

		mesh.vao->Bind();
		this->torusProgram->Use();
		this->torusProgram->SetMat4("worldMatrix", worldMtx);
		this->torusProgram->SetVec3("color", Utils::GetObjectColor(selectable.selected));
		glDrawElements(GL_LINES, 4 * torusComp.minorSegments * torusComp.majorSegments, static_cast<GLenum>(mesh.ebo->GetDataType()), static_cast<void*>(0));
	}
}
