#pragma once
#include <gl/glew.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <functional>

namespace GL
{
	class Program
	{
	private:
		static GLuint currentlyUsedProgramID;
//		static std::unordered_map<unsigned int, GLuint> usedUboBindingPoints;
//
//		std::unordered_map<std::string, GLuint> uniformBlockIdxMap;
//		std::unordered_map<std::string, GLint> uniformVariableIdxMap;
//
	private:
		GLuint ID;

		void Build(Shader* vertexShader, Shader* geometryShader, Shader* fragmentShader);
		void ExecuteUniformVariableOperation(std::string uniformName, std::function<void(GLint)> operation);

	public:
//		// https://gamedev.stackexchange.com/questions/68482/glgetuniformlocation-returns-1
//		static InvalidUniformAction invalidUniformAction;
//
		Program(Shader& vertexShader, Shader& fragmentShader);
//		Program(Shader& vertexShader, Shader& geometryShader, Shader& fragmentShader);
		~Program();

		void Use();
		constexpr GLuint GetID() const { return this->ID; };

		void SetMat4(std::string uniformName, glm::mat4 value);
		void SetVec3(std::string uniformName, glm::vec3 value);
//		void SetIVec3(std::string uniformName, glm::ivec3 value);
		void SetVec4(std::string uniformName, glm::vec4 value);
//		void SetIVec4(std::string uniformName, glm::ivec4 value);
		void SetInt(std::string uniformName, int value);
		void SetUint(std::string uniformName, unsigned int value);
//		void SetFloat(std::string uniformName, float value);
//
//		void SetVec3Array(std::string uniformName, const std::vector<glm::vec3>& values);
//		template<int N>
//		void SetVec3Array(std::string uniformName, const std::array<glm::vec3, N>& values)
//		{
//			this->ExecuteUniformVariableOperation(
//				uniformName,
//				[&](GLint location) { glUniform3fv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const GLfloat*>(values.data())); }
//			);
//		}
//
//		void SetIVec3Array(std::string uniformName, const std::vector<glm::ivec3>& values);
//		template<int N>
//		void SetIVec3Array(std::string uniformName, const std::array<glm::ivec3, N>& values)
//		{
//			this->ExecuteUniformVariableOperation(
//				uniformName,
//				[&](GLint location) { glUniform3iv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const GLint*>(values.data())); }
//			);
//		}
	};
}
