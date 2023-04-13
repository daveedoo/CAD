#include "BezierC0System.h"
#include "../../gl/ProgramFactory.h"
#include <iostream>


BezierC0System::BezierC0System(std::shared_ptr<entt::registry> registry, std::shared_ptr<CameraMovementInputHandler> cameraHandler, std::shared_ptr<ICurveSegmentsMetrics> curveSegmentsMetrics)
	: System(std::move(registry)),
	program(ProgramFactory::CreateProgram("torus.vert", "torus.frag")),
	cameraHandler(std::move(cameraHandler)),
	curveSegmentsMetrics(std::move(curveSegmentsMetrics))
{
	this->registry->on_construct<BezierC0>().connect<&BezierC0System::UpdateCurveMesh>(*this);
}

void BezierC0System::Update(const Camera& camera)
{
	if (cameraHandler->IsCameraMoving())
	{
		auto view = this->registry->view<BezierC0, Mesh>();
		for (auto [entity, bezier, mesh] : view.each())
		{
			UpdateCurveMesh(*this->registry, entity);
		}
	}
}

void BezierC0System::Render(const Camera& camera)
{
	this->program->Use();
	this->program->SetMat4("worldMatrix", glm::mat4(1.f));
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());
	this->program->SetVec3("color", glm::vec3(1.f));
	glLineWidth(1.f);
	glPointSize(0.5f);
	
	auto view = this->registry->view<BezierC0, Mesh>();
	for (auto [entity, bezier, mesh] : view.each())
	{
		mesh.vao->Bind();
		glDrawElements(GL_LINES, mesh.ebo->GetNrOfElements(), static_cast<GLenum>(mesh.ebo->GetDataType()), static_cast<void*>(0));
	}
}

glm::vec3 BezierC0System::CalculateBezierValue(const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3, float t)
{
	if (t < 0 || t > 1)
		throw std::invalid_argument("BezierC0System::CalculateBezierValue - invalid 't' arg");
	
	glm::vec3 b1_0 = b1 * t + b0 * (1 - t);
	glm::vec3 b1_1 = b2 * t + b1 * (1 - t);
	glm::vec3 b1_2 = b3 * t + b2 * (1 - t);

	glm::vec3 b2_0 = b1_1 * t + b1_0 * (1 - t);
	glm::vec3 b2_1 = b1_2 * t + b1_1 * (1 - t);

	glm::vec3 b3_0 = b2_1 * t + b2_0 * (1 - t);

	return b3_0;
}

std::vector<glm::vec3> BezierC0System::CalculateBezierValues(const BezierC0& bezier)
{
	std::vector<glm::vec3> points;
	for (size_t i = 0; i < bezier.points.size() / 4; i++)
	{
		// TODO: ensure its a Point
		auto b0 = this->registry->get<Position>(bezier.points[4 * i + 0]).position;
		auto b1 = this->registry->get<Position>(bezier.points[4 * i + 1]).position;
		auto b2 = this->registry->get<Position>(bezier.points[4 * i + 2]).position;
		auto b3 = this->registry->get<Position>(bezier.points[4 * i + 3]).position;
		auto arr = std::array<glm::vec3, 4>{ b0, b1, b2, b3 };

		unsigned int segments = curveSegmentsMetrics->CalculateSegmentsCount(arr);
		for (size_t j = 0; j <= segments; j++)
		{
			points.push_back(BezierC0System::CalculateBezierValue(b0, b1, b2, b3, static_cast<float>(j) / segments));
		}
	}

	return points;
}

void BezierC0System::UpdateCurveMesh(entt::registry& registry, entt::entity entity)
{
	const auto& bezier = registry.get<BezierC0>(entity);
	auto points = CalculateBezierValues(bezier);

	auto vao = std::make_unique<GL::VAO>();
	auto ebo = std::make_unique<GL::EBO>();
	auto vbo = std::make_unique<GL::VBO>(points.data(), points.size() * sizeof(glm::vec3));

	std::vector<unsigned int> idx;
	for (size_t i = 0; i < points.size() - 1; i++)
	{
		idx.push_back(i);
		idx.push_back(i + 1);
	}

	ebo->SetBufferData(idx.data(), GL::EBO::DataType::UINT, idx.size());
	vao->DefineFloatAttribute(*vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);

	registry.emplace_or_replace<Mesh>(entity, std::move(vao), std::move(vbo), std::move(ebo));
}
