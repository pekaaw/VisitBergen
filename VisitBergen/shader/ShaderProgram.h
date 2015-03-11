#pragma once

#include <map>
#include <string>

#include "..\util\includeGL.h"
#include "..\renderer\GLState.h"

struct GLState;

struct ShaderProgram
{
	GLuint shaderProgram;

	std::map<std::string, GLint> uniformIDs;
	std::map<std::string, GLint> attribLocations;

	void initMaterialUniforms();
	void initLightUniforms();
	void initModelViewProjectionUniforms();

	void initAttribLocations();

	void updateAllUniforms(const GLState& state);
	void updateMaterialUniforms(const GLState& state);
	void updateLightUniforms(const GLState& state);
	void updateModelViewProjectionUniforms(const GLState& state);

	void updateModelUniform(const glm::mat4& modelMatrix);
	void updateViewUniform(const glm::mat4& viewMatrix);
	void updateProjectionUniform(const glm::mat4& projectionMatrix);

	~ShaderProgram()
	{
		glDeleteProgram(this->shaderProgram);
	}

private:
	void initUniform(const std::string &name);
	void initAttribLocation(const std::string &name);
};