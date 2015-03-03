#include <glm\gtc\matrix_transform.hpp>

#include "..\..\thirdParty\lodepng.h"

#include "ContainerOBJ.h"
#include "..\..\renderer\Renderer.h"

ContainerOBJ::ContainerOBJ() :
vertexBufferObject(0),
indexBufferObject(0),
textureObject(0)
{
	this->model.reset();
	this->modelMatrix = glm::mat4();
	//this->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	//this->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, -0.0f));
}

ContainerOBJ::~ContainerOBJ()
{}

bool ContainerOBJ::init(const char* modelObjPath)
{
	if (!this->model)
	{
		this->model = std::make_shared<ModelOBJ>();
	}

	if (!this->model->import(modelObjPath))
	{
		printf("Unable to load model: %s \n", modelObjPath);
		return false;
	}

	this->model->normalize();

	// Generate buffer and fill with vertices
	glGenBuffers(1, &this->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,
		this->model->getNumberOfVertices() * sizeof(ModelOBJ::Vertex),
		this->model->getVertexBuffer(),
		GL_STATIC_DRAW);

	// Generate buffer and fill with indices
	glGenBuffers(1, &this->indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		3 * this->model->getNumberOfTriangles() * sizeof(int),
		this->model->getIndexBuffer(),
		GL_STATIC_DRAW);


	// Init materials
	unsigned char* textureData;
	unsigned int textureWidth;
	unsigned int textureHeight;
	for (int i = 0; i < this->model->getNumberOfMaterials(); ++i)
	{
		// if the current material has a texture
		if (this->model->getMaterial(i).colorMapFilename != "")
		{
			// load the texture
			unsigned int result = lodepng_decode_file(&textureData, 
				&textureWidth,
				&textureHeight,
				this->model->getMaterial(i).colorMapFilename.c_str(),
				LCT_RGB,
				8);

			if (result != 0)
			{
				printf("Error: ContainerOBJ::init was unable to load texture: %s \n",
					this->model->getMaterial(i).colorMapFilename.c_str());
				printf("LodePNG says %u \n", result);
				return false;
			}

			if (this->textureObject != 0)
			{
				glDeleteTextures(1, &this->textureObject);
			}
			glGenTextures(1, &this->textureObject);

			glBindTexture(GL_TEXTURE_2D, this->textureObject);

			glTexImage2D(
				GL_TEXTURE_2D,
				0, 
				GL_RGB,
				textureWidth,
				textureHeight,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				textureData);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			free(textureData);

			break; // only one texture for now. Todo: support several textures later!
		}
	}



	return true;
} /* Mesh::init(inputFile) */

void ContainerOBJ::draw()
{
	
	GLint positionLocation = glGetAttribLocation(Renderer::instance->getShaderProgram(), "position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);

	glDrawElements(
		GL_TRIANGLES,
		this->model->getNumberOfIndices(),
		GL_UNSIGNED_INT,
		0);

	glDisableVertexAttribArray(positionLocation);
}

glm::mat4 const ContainerOBJ::getModelMatrix() const
{
	return this->modelMatrix;
}