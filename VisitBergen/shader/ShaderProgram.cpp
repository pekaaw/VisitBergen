#include "ShaderProgram.h"

void ShaderProgram::initUniform(const std::string& name)
{
	uniformIDs[name] = glGetUniformLocation(shaderProgram, name.c_str());
}

void ShaderProgram::initMaterialUniforms()
{
	initUniform("material.ambient");
	initUniform("material.diffuse");
	initUniform("material.specular");
	initUniform("material.shininess");
}

void ShaderProgram::initLightUniforms()
{
	initUniform("light.position");
	initUniform("light.ambient");
	initUniform("light.diffuse");
	initUniform("light.specular");
}

void ShaderProgram::initModelViewProjectionUniforms()
{
	initUniform("ModelMatrix");
	initUniform("ViewMatrix");
	initUniform("ProjectionMatrix");
	initUniform("NormalMatrix");
}

void ShaderProgram::updateAllUniforms(const GLState &state)
{
	updateMaterialUniforms(state);
	updateLightUniforms(state);
	updateModelViewProjectionUniforms(state);
}

void ShaderProgram::updateMaterialUniforms(const GLState &state)
{
	glUniform3fv(uniformIDs["material.ambient"], 1, &state.material.ambient[0]);
	glUniform3fv(uniformIDs["material.diffuse"], 1, &state.material.diffuse[0]);
	glUniform3fv(uniformIDs["material.specular"], 1, &state.material.specular[0]);
	glUniform1fv(uniformIDs["material.shininess"], 1, &state.material.shininess);
}

void ShaderProgram::updateLightUniforms(const GLState &state)
{
	glUniform4fv(uniformIDs["light.position"], 1, &state.light.position[0]);
	glUniform3fv(uniformIDs["light.ambient"], 1, &state.light.ambient[0]);
	glUniform3fv(uniformIDs["light.diffuse"], 1, &state.light.diffuse[0]);
	glUniform3fv(uniformIDs["light.specular"], 1, &state.light.specular[0]);
}

void ShaderProgram::updateModelViewProjectionUniforms(const GLState &state)
{
	glm::mat3 normalMatrix(glm::transpose(glm::inverse(state.viewMatrix)));
	glUniformMatrix4fv(uniformIDs["ModelMatrix"], 1, GL_FALSE, &state.modelMatrix[0][0]);
	glUniformMatrix4fv(uniformIDs["ViewMatrix"], 1, GL_FALSE, &state.viewMatrix[0][0]);
	glUniformMatrix4fv(uniformIDs["ProjectionMatrix"], 1, GL_FALSE, &state.projectionMatrix[0][0]);
	glUniformMatrix3fv(uniformIDs["NormalMatrix"], 1, GL_FALSE, &normalMatrix[0][0]);
}