#include "Shader.h"
#include <iostream>
#include <fstream>

namespace GL
{
	Shader::Shader(ShaderType type, std::filesystem::path shaderSourcePath)
		: type(type)
	{
		std::string shaderSourceCode = this->ReadFileContents(shaderSourcePath);
		this->ID = this->CompileShader(shaderSourceCode);
	}

	Shader::~Shader()
	{
		glDeleteShader(this->ID);
	}

	std::string Shader::ReadFileContents(std::filesystem::path shaderSourcePath)
	{
		std::ifstream shaderSourceFile(shaderSourcePath, std::ios::binary);
		if (!shaderSourceFile)
		{
			std::string excMsg = std::format("Shader::ReadFileContents - could not open shader source file: \"{}\"", shaderSourcePath.string());
			throw std::exception(excMsg.c_str());
		}
		std::string shaderSourceCode;
		shaderSourceFile.seekg(0, std::ios::end);
		shaderSourceCode.resize(shaderSourceFile.tellg());
		shaderSourceFile.seekg(0, std::ios::beg);
		shaderSourceFile.read(&shaderSourceCode[0], shaderSourceCode.size());
		if (!shaderSourceFile)
		{
			std::string excMsg = std::format("Shader::ReadFileContents - could not read shader source file: \"{}\"", shaderSourcePath.string());
			throw std::exception(excMsg.c_str());
		}
		shaderSourceFile.close();
		return shaderSourceCode;
	}

	GLuint Shader::CompileShader(std::string shaderSourceCode)
	{
		const GLchar* sourceCode = shaderSourceCode.c_str();
		GLuint shaderID = glCreateShader(static_cast<GLenum>(this->type));
		glShaderSource(shaderID, 1, &sourceCode, NULL);
		glCompileShader(shaderID);
		int compilationStatus;
		GLchar compilationInfo[2048];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus);
		if (compilationStatus == GL_FALSE)
		{
			glGetShaderInfoLog(shaderID, 2048, NULL, compilationInfo);
			std::string excMsg = std::format("Shader::CompileShader - could not compile shader. Compilation info:\n{}", compilationInfo);
			throw std::exception(excMsg.c_str());
		}
		return shaderID;
	}
}
