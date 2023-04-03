#include "ProgramFactory.h"

const std::filesystem::path ProgramFactory::SHADERS_PATH = std::filesystem::path(SHADERS_DIR);

std::unique_ptr<GL::Program> ProgramFactory::CreateProgram(std::string vertexShader, std::string fragmentShader)
{
	GL::Shader VS(GL::Shader::ShaderType::VERTEX_SHADER, SHADERS_PATH / vertexShader);
	GL::Shader FS(GL::Shader::ShaderType::FRAGMENT_SHADER, SHADERS_PATH / fragmentShader);
	auto program = std::make_unique<GL::Program>(VS, FS);
	return std::move(program);
}
