#pragma once
#include "System.h"
#include "../../gl/wrappers/Program.h"
#include "../../Window/input/handlers/CameraMovementInputHandler.h"
#include "CurveSegmentsMetrics/ICurveSegmentsMetrics.h"
#include <tuple>
#include "..\objects\Components\BezierC0.h"

class BezierC0System : public System, public CameraSubscriber
{
private:
	std::unique_ptr<GL::Program> program;
	std::shared_ptr<CameraMovementInputHandler> cameraHandler;
	std::shared_ptr<ICurveSegmentsMetrics> curveSegmentsMetrics;

public:
	BezierC0System(std::shared_ptr<entt::registry> registry, std::shared_ptr<CameraMovementInputHandler> cameraHandler, std::shared_ptr<ICurveSegmentsMetrics> curveSegmentsMetrics);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	// Inherited via CameraSubscriber
	virtual void OnCameraMove() override;

private:
	static glm::vec3 CalculateBezierValue(const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3, float t);
	std::vector<glm::vec3> CalculateBezierValues(const BezierC0& bezier);
	
	std::tuple<std::vector<glm::vec3>, std::vector<unsigned int>> GetCurveMeshData(const BezierC0& bezier);
	void SetCurveMesh(entt::registry& registry, entt::entity bezierEntity);
	void UpdateCurveMesh(entt::registry& registry, entt::entity bezierEntity);

	void UpdateAllBeziersContainingPoint(entt::registry& registry, entt::entity positionEntity);
	void RemovePointFromBeziers(entt::registry& registry, entt::entity pointEntity);
};
