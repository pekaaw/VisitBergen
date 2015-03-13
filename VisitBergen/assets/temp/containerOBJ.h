#pragma once

#include <memory>
#include <unordered_map>

#include <glm\glm.hpp>

#include "..\..\util\includeGL.h"
#include "..\..\thirdParty\model_obj.h"
#include "..\..\shader\ShaderProgram.h"

class ContainerOBJ
{
public:
	ContainerOBJ();
	~ContainerOBJ();

	bool init(const char* inputFile);
	void draw();

	glm::mat4 const getModelMatrix() const;
	void setShaderProgram(const std::shared_ptr<ShaderProgram> shaderProgram);
	void toggleRotation();

private:
	std::shared_ptr<ModelOBJ> model;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;

	// Turn rotation during drawing on / off
	bool runRotation;

	// uniform location for bool NoTexture
	GLint uNoTexture;

	// ModelMatrix for this particular model
	glm::mat4 modelMatrix;

	// map texture name to texture location on GPU
	std::unordered_map<std::string, GLuint> textureObjects;

	std::shared_ptr<ShaderProgram> shaderProgram;

	void averageVertexNormals();
};