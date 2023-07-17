#include "CursorSystem.h"
#include "../../gl/ProgramFactory.h"
#include "..\objects\Components\Cursor.h"
#include "..\objects\Components\Transformation.h"

CursorSystem::CursorSystem(std::shared_ptr<entt::registry> registry) : System(registry),
	program(ProgramFactory::CreateProgram("coloredVertices.vert", "coloredVertices.frag")),
	cursorMesh(std::make_unique<Mesh>(Mesh::Cursor(0.5f)))
{
}

void CursorSystem::Update(const Camera& camera)
{
}

void CursorSystem::Render(const Camera& camera)
{
	this->program->Use();
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());

	// torus cursors
	auto view = this->registry->view<Cursor, Transformation>();
	for (auto [entity, cursor, Transformation] : view.each())
	{
		cursorMesh->vao->Bind();
		this->program->SetMat4("worldMatrix", Transformation.worldMatrix);

		glLineWidth(cursor.lineWidth);
		glDrawArrays(GL_LINES, 0, 6);	// TODO: move parameters to Mesh component
	}
}
