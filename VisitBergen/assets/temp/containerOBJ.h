#pragma once

#include <memory>
#include <vector>

#include <glm\glm.hpp>

#include "..\..\util\includeGL.h"
#include "..\..\thirdParty\model_obj.h"

class ContainerOBJ
{
public:
	ContainerOBJ();
	~ContainerOBJ();

	bool init(const char* inputFile);
	void draw();

	glm::mat4 const getModelMatrix() const;

private:
	std::shared_ptr<ModelOBJ> model;
	GLuint vertexBufferObject;
	GLuint indexBufferObject;

	glm::mat4 modelMatrix;

	GLuint textureObject;

	std::vector<GLuint> textureObjects;
};