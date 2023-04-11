#pragma once
#include "wrappers/Program.h"
#include "../Config.h"

class ProgramFactory
{
private:
	static const std::filesystem::path SHADERS_PATH;

public:
	static std::unique_ptr<GL::Program> CreateProgram(std::string vertexShader, std::string fragmentShader);
};

