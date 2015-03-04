#include <glm\gtc\matrix_transform.hpp>

#include "..\..\thirdParty\lodepng.h"

#include "ContainerOBJ.h"
#include "..\..\renderer\Renderer.h"

ContainerOBJ::ContainerOBJ() :
vertexBufferObject(0),
indexBufferObject(0)
{
	this->model.reset();
	this->modelMatrix = glm::mat4();
	//this->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	//this->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, -0.0f));
}

ContainerOBJ::~ContainerOBJ()
{
	// Delete vertex buffer object
	if (glIsBuffer(this->vertexBufferObject))
	{
		glDeleteBuffers(1, &this->vertexBufferObject);
	}

	// Delete index buffer object
	if (glIsBuffer(this->indexBufferObject))
	{
		glDeleteBuffers(1, &this->indexBufferObject);
	}

	// delete all textures
	for (auto it = this->textureObjects.begin(); it != this->textureObjects.end(); ++it)
	{
		GLuint texture = it->second;
		if (glIsTexture(texture))
		{
			glDeleteTextures(1, &texture);
		}
	}

	// clear the map with textures
	this->textureObjects.clear();

	// release the model
	this->model.reset();
}

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
	//glGenBuffers(1, &this->indexBufferObject);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//	3 * this->model->getNumberOfTriangles() * sizeof(int),
	//	this->model->getIndexBuffer(),
	//	GL_STATIC_DRAW);	
	
	glGenBuffers(1, &this->indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		this->model->getNumberOfIndices() * sizeof(unsigned int),
		this->model->getIndexBuffer(),
		GL_STATIC_DRAW);


	// Init materials
	//unsigned char* textureData;
	//unsigned int textureWidth;
	//unsigned int textureHeight;
	for (int i = 0; i < this->model->getNumberOfMaterials(); ++i)
	{
		// if the current material has a texture
		if (this->model->getMaterial(i).colorMapFilename != "")
		{
			const std::string* textureFileName = &(this->model->getMaterial(i)).colorMapFilename;

			std::vector<unsigned char> png;
			std::vector<unsigned char> image;
			unsigned width, height;

			lodepng::load_file(png, *textureFileName);
			unsigned int result = lodepng::decode(image, width, height, png);

			//// load the texture
			//unsigned int result = lodepng_decode_file(&textureData, 
			//	&textureWidth,
			//	&textureHeight,
			//	textureFileName->c_str(),
			//	LCT_RGB,
			//	8);

			if (result != 0)
			{
				printf("Error: ContainerOBJ::init was unable to load texture: %s. LodePNG says %u \n", textureFileName->c_str(), result);
				return false;
			}

			//if (this->textureObject != 0)
			//{
			//	glDeleteTextures(1, &this->textureObject);
			//}
			
			GLuint texture = 0;
			glGenTextures(1, &texture);
			//glGenTextures(1, &this->textureObject);

			glBindTexture(GL_TEXTURE_2D, texture);
			//glBindTexture(GL_TEXTURE_2D, this->textureObject);

			glTexImage2D(
				GL_TEXTURE_2D,
				0, 
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image.data());

			// add texture reference to texture map
			this->textureObjects[*textureFileName] = texture;

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// unbind texture, cleanup, set texture to NULL / 0
			glBindTexture(GL_TEXTURE_2D, 0);

			// Data loaded to GPU, can free it from HEAP
			//free(textureData);
			image.clear();
		}
	}



	return true;
} /* Mesh::init(inputFile) */

void ContainerOBJ::draw()
{
	GLint positionLocation = glGetAttribLocation(Renderer::getInstance()->getShaderProgram(), "position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(0));

	GLint texCoordsLocation = glGetAttribLocation(Renderer::getInstance()->getShaderProgram(), "tex_coords");
	glEnableVertexAttribArray(texCoordsLocation);
	glVertexAttribPointer(texCoordsLocation,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(3 * sizeof(float)));

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, this->textureObject);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);

	//glDrawElements(
	//	GL_TRIANGLES,
	//	this->model->getNumberOfIndices(),
	//	GL_UNSIGNED_INT,
	//	0);

	for (int m = 0; m < this->model->getNumberOfMeshes(); ++m)
	{
		const ModelOBJ::Mesh* mesh = &this->model->getMesh(m);
		const std::string colorMap = mesh->pMaterial->colorMapFilename;

		// reference for the GPU of which texture object to use. Default to 0;
		GLuint texture = 0;

		// If we find a texture object with the right name, then tell OpenGL to use it
		auto it = this->textureObjects.find(colorMap);
		if (it != this->textureObjects.end())
		{
			texture = it->second;
		}

		// set the active texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture); // get texture object from hashmap

		// select part of mesh to render
		int startIndex = mesh->startIndex;
		int numIndices = mesh->triangleCount * 3;

		// Draw the elements on the GPU
		glDrawElements(
			GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			reinterpret_cast<const GLvoid*>(startIndex * sizeof(GL_UNSIGNED_INT))); // size in bytes
	}

	glDisableVertexAttribArray(texCoordsLocation);
	glDisableVertexAttribArray(positionLocation);
}

glm::mat4 const ContainerOBJ::getModelMatrix() const
{
	return this->modelMatrix;
}