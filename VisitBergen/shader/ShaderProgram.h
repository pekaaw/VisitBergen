#pragma once

#include <map>
#include <string>

#include "..\util\includeGL.h"
#include "../renderer/GLState.h"

struct GLState;

struct ShaderProgram
{
	GLuint shaderProgram;

	std::map<std::string, GLint> uniformIDs;

	void initUniform(const std::string &name);
	void initMaterialUniforms();
	void initLightUniforms();
	void initModelViewProjectionUniforms();

	void updateAllUniforms(const GLState& state);
	void updateMaterialUniforms(const GLState& state);
	void updateLightUniforms(const GLState& state);
	void updateModelViewProjectionUniforms(const GLState& state);
};