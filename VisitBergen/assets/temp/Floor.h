#pragma once

#include <memory>

#include <glm\glm.hpp>

#include "..\..\util\includeGL.h"
#include "..\..\shader\ShaderProgram.h"

class Floor
{
public:
	Floor();
	~Floor();

	void init();
	void draw();
	void setShaderProgram(const std::shared_ptr<ShaderProgram> shaderProgram);

private:
	std::shared_ptr<ShaderProgram> shaderProgram;
	GLuint vertexBuffer;
	GLuint indexBuffer;

	glm::mat4 modelMatrix;
};

