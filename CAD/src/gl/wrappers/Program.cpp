#include "Program.h"
#include <glm/gtc/type_ptr.hpp>
//#include <iostream>

namespace GL
{
	GLuint Program::currentlyUsedProgramID = 0;

	Program::Program(Shader& vertexShader, Shader& fragmentShader)
	{
		this->Build(&vertexShader, nullptr, &fragmentShader);
	}

	void Program::Build(Shader* vertexShader, Shader* geometryShader, Shader* fragmentShader)
	{
		if (vertexShader == nullptr || vertexShader->type != Shader::ShaderType::VERTEX_SHADER)
		{
			std::string excMsg = std::format("Program::Build - vertexShader is null or not of type VERTEX_SHADER");
			throw std::invalid_argument(excMsg);
		}
		if (geometryShader != nullptr && geometryShader->type != Shader::ShaderType::GEOMETRY_SHADER)
		{
			std::string excMsg = std::format("Program::Build - geometryShader argument is not of type GEOMETRY_SHADER");
			throw std::invalid_argument(excMsg);
		}
		if (fragmentShader == nullptr || fragmentShader->type != Shader::ShaderType::FRAGMENT_SHADER)
		{
			std::string excMsg = std::format("Program::Build - fragmentShader is null or not of type FRAGMENT_SHADER");
			throw std::invalid_argument(excMsg);
		}

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertexShader->GetID());
		if (geometryShader != nullptr)
		{
			glAttachShader(this->ID, geometryShader->GetID());
		}
		glAttachShader(this->ID, fragmentShader->GetID());
		glLinkProgram(this->ID);

		int shaderLinkingStatus;
		GLchar shaderLinkingInfo[512];
		glGetProgramiv(this->ID, GL_LINK_STATUS, &shaderLinkingStatus);
		if (!shaderLinkingStatus)
		{
			glGetProgramInfoLog(this->ID, 512, NULL, shaderLinkingInfo);
			std::string excMsg = std::format("Program::Program - shader linking error:\n{}", shaderLinkingInfo);
			throw std::exception(excMsg.c_str());
		}

		glDetachShader(this->ID, vertexShader->GetID());
		if (geometryShader != nullptr)
		{
			glDetachShader(this->ID, geometryShader->GetID());
		}
		glDetachShader(this->ID, fragmentShader->GetID());
	}

	Program::~Program()
	{
		glDeleteProgram(this->ID);
		if (currentlyUsedProgramID == this->ID)
			currentlyUsedProgramID = 0;
	}

	void Program::Use()
	{
		if (currentlyUsedProgramID != this->ID)
		{
			glUseProgram(this->ID);
			currentlyUsedProgramID = this->ID;
		}
	}

	void Program::ExecuteUniformVariableOperation(std::string uniformName, std::function<void(GLint)> operation)
	{
		this->Use();
		GLint location = glGetUniformLocation(this->ID, uniformName.c_str());
		if (location == -1)
		{
			throw std::invalid_argument(std::format(
				"Program::ExecuteUniformVariableOperation - unable to locate uniform variable \"{}\".", uniformName));
		}

		operation(location);
	}

	void Program::SetMat4(std::string uniformName, glm::mat4 value)
	{
		this->ExecuteUniformVariableOperation(uniformName,
			[&](GLint location)
			{ glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)); });
	}

	void Program::SetVec3(std::string uniformName, glm::vec3 value)
	{
		this->ExecuteUniformVariableOperation(uniformName,
			[&](GLint location) { glUniform3f(location, value.x, value.y, value.z); }
		);
	}

//	void Program::SetIVec3(std::string uniformName, glm::ivec3 value)
//	{
//		this->ExecuteUniformVariableOperation(
//			uniformName,
//			[&](GLint location) { glUniform3i(location, value.x, value.y, value.z); }
//		);
//	}

	void Program::SetVec4(std::string uniformName, glm::vec4 value)
	{
		this->ExecuteUniformVariableOperation(
			uniformName,
			[&](GLint location) { glUniform4f(location, value.x, value.y, value.z, value.w); }
		);
	}
//
//	void Program::SetIVec4(std::string uniformName, glm::ivec4 value)
//	{
//		this->ExecuteUniformVariableOperation(
//			uniformName,
//			[&](GLint location) { glUniform4i(location, value.x, value.y, value.z, value.w); }
//		);
//	}
// 
//	void Program::SetMat4(std::string uniformName, glm::mat4 value)
//	{
//		this->ExecuteUniformVariableOperation(
//			uniformName,
//			[&](GLint location) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)); }
//		);
//	}

	void Program::SetInt(std::string uniformName, int value)
	{
		this->ExecuteUniformVariableOperation(uniformName,
			[&](GLint location) { glUniform1i(location, value); }
		);
	}

	void Program::SetUint(std::string uniformName, unsigned int value)
	{
		this->ExecuteUniformVariableOperation(uniformName,
			[&](GLint location) { glUniform1ui(location, value); }
		);
	}

	void Program::SetFloat(std::string uniformName, float value)
	{
		this->ExecuteUniformVariableOperation(
			uniformName,
			[&](GLint location) { glUniform1f(location, value); }
		);
	}
//
//	void Program::SetVec3Array(std::string uniformName, const std::vector<glm::vec3>& values)
//	{
//		this->ExecuteUniformVariableOperation(
//			uniformName,
//			[&](GLint location) { glUniform3fv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const GLfloat*>(values.data())); }
//		);
//	}
//
//	void Program::SetIVec3Array(std::string uniformName, const std::vector<glm::ivec3>& values)
//	{
//		this->ExecuteUniformVariableOperation(
//			uniformName,
//			[&](GLint location) { glUniform3iv(location, static_cast<GLsizei>(values.size()), reinterpret_cast<const GLint*>(values.data())); }
//		);
//	}
}
