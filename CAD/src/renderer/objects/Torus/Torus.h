#pragma once
#include <memory>
#include "../../../gl/wrappers/VAO.h"
#include "../../../gl/wrappers/Program.h"
#include "../../../Camera.h"
#include "../../../gl/wrappers/EBO.h"

class Torus
{
private:
	float minorR;
	float majorR;
	int minorSegments;
	int majorSegments;

	glm::vec3 translation;
	glm::vec3 scale = glm::vec3(1.f);
	float rotX = 0.f, rotY = 0.f, rotZ = 0.f;
	glm::mat4x4 worldMatrix;


	glm::vec3 color;

	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::EBO> ebo;
	std::unique_ptr<GL::Program> program;

	static glm::vec3 GetTorusPoint(float majorAngleRad, float minorAngleRad, float majorR, float minorR);
	void UpdateWorldMatrix();

public:
	Torus(float minorR, float majorR, unsigned int minorSegments, unsigned int majorSegments);

	float GetMinorR() { return this->minorR; }
	float GetMajorR() { return this->majorR; }
	int GetMinorSegments() { return this->minorSegments; }
	int GetMajorSegments() { return this->majorSegments; }
	void SetMinorR(float val);
	void SetMajorR(float val);
	void SetMinorSegments(int val);
	void SetMajorSegments(int val);

	void SetTranslation(glm::vec3 v);
	void SetScale(glm::vec3 scale);
	void SetRotationX(float angleDeg);
	void SetRotationY(float angleDeg);
	void SetRotationZ(float angleDeg);

	void SetBuffers();
	void Render(const Camera& camera);
};
