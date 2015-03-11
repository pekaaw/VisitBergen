#pragma once

#include <memory>
#include <string>

#include "ShaderProgram.h"


class ShaderFactory
{
public:
	ShaderFactory(){}
	~ShaderFactory(){}

	std::shared_ptr<ShaderProgram> makeShaderProgram(const std::string &shaderName);

private:
	GLuint initGLProgram(const std::string &shaderProgram);
};