#pragma once
#include <gl/glew.h>
#include <filesystem>

namespace GL
{
	class Shader
	{
	public:
		enum class ShaderType
		{
			VERTEX_SHADER = GL_VERTEX_SHADER,
			GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		};

	private:
		GLuint ID;

		std::string ReadFileContents(std::filesystem::path shaderSourcePath);
		GLuint CompileShader(std::string shaderSourceCode);

	public:
		const ShaderType type;

		Shader(ShaderType type, std::filesystem::path shaderSourcePath);
		~Shader();

		GLuint GetID() const { return this->ID; }
	};
}
