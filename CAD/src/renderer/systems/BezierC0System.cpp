#include "BezierC0System.h"
#include "../../gl/ProgramFactory.h"
#include <iostream>
#include <algorithm>

BezierC0System::BezierC0System(std::shared_ptr<entt::registry> registry, std::shared_ptr<CameraMovementInputHandler> cameraHandler, std::shared_ptr<ICurveSegmentsMetrics> curveSegmentsMetrics)
	: System(std::move(registry)),
	program(ProgramFactory::CreateProgram("torus.vert", "torus.frag")),
	cameraHandler(std::move(cameraHandler)),
	curveSegmentsMetrics(std::move(curveSegmentsMetrics))
{
	this->registry->on_construct<BezierC0>().connect<&BezierC0System::SetCurveMesh>(*this);
	this->registry->on_update<BezierC0>().connect<&BezierC0System::UpdateCurveMesh>(*this);
	this->registry->on_update<Position>().connect<&BezierC0System::UpdateAllBeziersContainingPoint>(*this);
	this->registry->on_destroy<Point>().connect<&BezierC0System::RemovePointFromBeziers>(*this);
}

void BezierC0System::Update(const Camera& camera)
{
}

void BezierC0System::Render(const Camera& camera)
{
	this->program->Use();
	this->program->SetMat4("worldMatrix", glm::mat4(1.f));
	this->program->SetMat4("viewMatrix", camera.GetViewMatrix());
	this->program->SetMat4("projMatrix", camera.GetProjectionMatrix());
	this->program->SetVec3("color", glm::vec3(1.f));
	glLineWidth(1.f);
	
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

	size_t size = bezier.points.size();
	if (size > 3)
		for (size_t i = 0; i < size - 3; i+=3)
		{
			// TODO: ensure its a Point
			auto b0 = this->registry->get<Position>(bezier.points[i + 0]).position;
			auto b1 = this->registry->get<Position>(bezier.points[i + 1]).position;
			auto b2 = this->registry->get<Position>(bezier.points[i + 2]).position;
			auto b3 = this->registry->get<Position>(bezier.points[i + 3]).position;
			auto arr = std::array<glm::vec3, 4>{ b0, b1, b2, b3 };

			unsigned int segments = curveSegmentsMetrics->CalculateSegmentsCount(arr);
			for (size_t j = 0; j <= segments; j++)
			{
				points.push_back(BezierC0System::CalculateBezierValue(b0, b1, b2, b3, static_cast<float>(j) / segments));
			}
		}

	return points;
}

std::tuple<std::vector<glm::vec3>, std::vector<unsigned int>> BezierC0System::GetCurveMeshData(const BezierC0& bezier)
{
	auto points = CalculateBezierValues(bezier);

	std::vector<unsigned int> idx;
	if (points.size() > 0)
		for (size_t i = 0; i < points.size() - 1; i++)
		{
			idx.push_back(i);
			idx.push_back(i + 1);
		}

	return std::make_tuple(points, idx);
}

void BezierC0System::SetCurveMesh(entt::registry& registry, entt::entity bezierEntity)
{
	const auto& bezier = registry.get<BezierC0>(bezierEntity);
	auto [points, idx] = GetCurveMeshData(bezier);

	auto vao = std::make_unique<GL::VAO>();
	auto ebo = std::make_unique<GL::EBO>();
	auto vbo = std::make_unique<GL::VBO>(points.data(), points.size() * sizeof(glm::vec3));
	ebo->SetBufferData(idx.data(), GL::EBO::DataType::UINT, idx.size());
	vao->DefineFloatAttribute(*vbo, 0, 3, GL::VAO::FloatAttribute::FLOAT, sizeof(glm::vec3), 0);

	registry.emplace<Mesh>(bezierEntity, std::move(vao), std::move(vbo), std::move(ebo));
}

void BezierC0System::UpdateCurveMesh(entt::registry& registry, entt::entity bezierEntity)
{
	const auto& bezier = registry.get<BezierC0>(bezierEntity);
	auto [points, idx] = GetCurveMeshData(bezier);

	this->registry->patch<Mesh>(bezierEntity, [&](Mesh& mesh) -> void
		{
			// TODO: remove line woth vao. EBO should be a part of VAO
			mesh.vao->Bind();
			mesh.vbo->SetBufferData(points.data(), points.size() * sizeof(glm::vec3));
			mesh.ebo->SetBufferData(idx.data(), GL::EBO::DataType::UINT, idx.size());
		});
}

void BezierC0System::UpdateAllBeziersContainingPoint(entt::registry& registry, entt::entity positionEntity)
{
	if (!registry.all_of<Point>(positionEntity))
		return;

	auto view = this->registry->view<BezierC0>();
	for (auto [bezierEntity, bezier] : view.each())
	{
		if (std::find(bezier.points.begin(), bezier.points.end(), positionEntity) != bezier.points.end())
			UpdateCurveMesh(registry, bezierEntity);
	}
}

void BezierC0System::RemovePointFromBeziers(entt::registry& registry, entt::entity pointEntity)
{
	auto view = registry.view<BezierC0>();
	for (auto [bezierEntity, bezier] : view.each())
	{
		auto newEnd = std::remove(bezier.points.begin(), bezier.points.end(), pointEntity);
		if (newEnd != bezier.points.end())
		{
			bezier.points.erase(newEnd, bezier.points.end());
			UpdateCurveMesh(registry, bezierEntity);
		}
	}
}

void BezierC0System::OnCameraMove()
{
	auto view = this->registry->view<BezierC0, Mesh>();
	for (auto [entity, bezier, mesh] : view.each())
	{
		UpdateCurveMesh(*this->registry, entity);
	}
}
