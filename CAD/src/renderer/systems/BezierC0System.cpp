#include "BezierC0System.h"
#include "../../gl/ProgramFactory.h"
#include <algorithm>
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Point.h"
#include "..\objects\Components\Mesh.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\maths\Matrix.h"
#include "..\..\maths\Bernstein.h"
#include "..\objects\Components\Selectable.h"
#include "..\..\..\Utils.h"

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
	glLineWidth(1.f);
	
	auto view = this->registry->view<BezierC0, Mesh, Selectable>();
	for (auto [entity, bezier, mesh, selectable] : view.each())
	{
		this->program->SetVec3("color", glm::vec3(1.f));
		mesh.vao->Bind();
		
		size_t curvePointsCount = mesh.ebo->GetNrOfElements() - bezier.points.size() + 1;
		auto dataType = mesh.ebo->GetDataType();
		glDrawElements(GL_LINE_STRIP, curvePointsCount, static_cast<GLenum>(dataType), (void*)(0));

		if (bezier.polylineVisible)
		{
			if (selectable.selected)
				this->program->SetVec3("color", Utils::GetObjectColor(true));
			glDrawElements(GL_LINE_STRIP, bezier.points.size(), static_cast<GLenum>(dataType),
				(void*)((curvePointsCount - 1) * GL::EBO::GetSizeOf(dataType)));
		}
	}
}

std::vector<glm::vec3> BezierC0System::CalculateBezierValues(const BezierC0& bezier)
{
	std::vector<glm::vec3> points;

	size_t size = bezier.points.size();
	if (size > 3)
	{
		for (size_t i = 0; i < size - 3; i += 3)
		{
			// TODO: ensure its a Point
			auto& b0_transf = this->registry->get<Transformation>(bezier.points[i + 0]);
			auto& b1_transf = this->registry->get<Transformation>(bezier.points[i + 1]);
			auto& b2_transf = this->registry->get<Transformation>(bezier.points[i + 2]);
			auto& b3_transf = this->registry->get<Transformation>(bezier.points[i + 3]);
			auto b0 = Matrix::ExtractTranslation(b0_transf.worldMatrix);
			auto b1 = Matrix::ExtractTranslation(b1_transf.worldMatrix);
			auto b2 = Matrix::ExtractTranslation(b2_transf.worldMatrix);
			auto b3 = Matrix::ExtractTranslation(b3_transf.worldMatrix);
			auto arr = std::array<glm::vec3, 4>{ b0, b1, b2, b3 };

			unsigned int segments = curveSegmentsMetrics->CalculateSegmentsCount(arr);
			for (size_t j = 0; j <= segments; j++)
			{
				points.push_back(Bernstein::CalculateBernsteinValue(b0, b1, b2, b3, static_cast<float>(j) / segments));
			}
		}
	}

	return points;
}

std::tuple<std::vector<glm::vec3>, std::vector<unsigned int>> BezierC0System::GetCurveMeshData(const BezierC0& bezier)
{
	auto points = CalculateBezierValues(bezier);

	std::vector<unsigned int> idx;
	size_t i = 0;
	if (points.size() > 0)
	{
		for (; i < points.size() - 1; i++)
		{
			idx.push_back(i);
		}
	}

	size_t size = bezier.points.size();
	if (size > 3)
	{
		for (size_t j = ((size - 1) / 3) * 3; j > 2; j -= 3)
		{
			//auto pos0 = this->registry->get<Position>(bezier.points[j - 0]).position;	// first point is already calculated
			auto pos1 = this->registry->get<Position>(bezier.points[j - 1]).position;
			auto pos2 = this->registry->get<Position>(bezier.points[j - 2]).position;
			auto pos3 = this->registry->get<Position>(bezier.points[j - 3]).position;
			points.push_back(pos1);
			points.push_back(pos2);
			points.push_back(pos3);
			idx.push_back(i + 1);
			idx.push_back(i + 2);
			idx.push_back(i + 3);
			i += 3;
		}
	}

	return std::make_tuple(points, idx);
}

void BezierC0System::SetCurveMesh(entt::registry& registry, entt::entity bezierEntity)
{
	const auto& bezier = registry.get<BezierC0>(bezierEntity);
	auto [points, idx] = GetCurveMeshData(bezier);

	auto vao = std::make_unique<GL::VAO>();
	auto ebo = std::make_unique<GL::EBO>(*vao);
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
