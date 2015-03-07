#pragma once

#include <memory>
#include <unordered_map>

#include <glm\glm.hpp>

#include "..\..\util\includeGL.h"
#include "..\..\thirdParty\model_obj.h"

struct MaterialLocations
{
	GLint uAmbient;
	GLint uDiffuse;
	GLint uSpecular;
	GLint uShininess;
	GLint uAlpha;

	MaterialLocations() 
		: uAmbient(-1), uDiffuse(-1), uSpecular(-1), uShininess(-1), uAlpha(-1)
	{}
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

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

	//MaterialLocations uMaterial;

	Material material;
	GLint uMaterial;

	GLint uNoTexture;

	std::unordered_map<std::string, GLuint> textureObjects;
};