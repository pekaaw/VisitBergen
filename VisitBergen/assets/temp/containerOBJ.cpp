#include <glm\gtc\matrix_transform.hpp>

#include "..\..\thirdParty\lodepng.h"

#include "ContainerOBJ.h"
#include "..\..\renderer\Renderer.h"

ContainerOBJ::ContainerOBJ() :
vertexBufferObject(0),
indexBufferObject(0),
runRotation(false),
lightRotation(0.0f)
//uMaterial(0)
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
	if (this->shaderProgram == nullptr)
	{
		printf("Error: ShaderProgram not set before initializing model (%s).", modelObjPath);
		return false;
	}

	if (!this->model)
	{
		this->model = std::make_shared<ModelOBJ>();
	}

	if (!this->model->import(modelObjPath))
	{
		printf("Unable to load model: %s \n", modelObjPath);
		return false;
	}

	// Calculate and apply average normal per vertex
	//averageVertexNormals();


	// Normalize the model (max width|height|depth = 1 and origin in center of model
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
		this->model->getNumberOfIndices() * sizeof(unsigned int),
		this->model->getIndexBuffer(),
		GL_STATIC_DRAW);

	GLuint program = this->shaderProgram->shaderProgram;
	//GLuint program = Renderer::getInstance()->getShaderProgram();
	//this->uMaterial.uAmbient = glGetUniformLocation(program, "MaterialAmbientColor");
	//this->uMaterial.uDiffuse = glGetUniformLocation(program, "MaterialDiffuseColor");
	//this->uMaterial.uSpecular = glGetUniformLocation(program, "MaterialSpecularColor");
	//this->uMaterial.uShininess = glGetUniformLocation(program, "MaterialShininess");
	//this->uMaterial.uAlpha = glGetUniformLocation(program, "MaterialAlpha");
	//this->uMaterial = glGetUniformLocation(program, "material");
	this->uNoTexture = glGetUniformLocation(program, "NoTexture");

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
	if (this->runRotation)
	{
		this->modelMatrix = glm::rotate_slow(this->modelMatrix, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	this->lightRotation += 0.03f;

	glUseProgram(this->shaderProgram->shaderProgram);

	this->shaderProgram->updateModelUniform(this->modelMatrix);

	GLint vertexLocation = shaderProgram->attribLocations["vertex"];
	//GLint vertexLocation = glGetAttribLocation(Renderer::getInstance()->getShaderProgram(), "vertex");
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(0));

	//GLint texCoordsLocation = this->shaderProgram->attribLocations["tex_coords"];
	////GLint texCoordsLocation = glGetAttribLocation(Renderer::getInstance()->getShaderProgram(), "tex_coords");
	//glEnableVertexAttribArray(texCoordsLocation);
	//glVertexAttribPointer(texCoordsLocation,
	//	2,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(ModelOBJ::Vertex),
	//	reinterpret_cast<const GLvoid*>(3 * sizeof(float)));

	GLint vertexNormalLocation = this->shaderProgram->attribLocations["normal"];
	//GLint vertexNormalLocation = glGetAttribLocation(Renderer::getInstance()->getShaderProgram(), "normal");
	glEnableVertexAttribArray(vertexNormalLocation);
	glVertexAttribPointer(vertexNormalLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(5 * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);

	for (int m = 0; m < this->model->getNumberOfMeshes(); ++m)
	{
		const ModelOBJ::Mesh* mesh = &this->model->getMesh(m);
		const ModelOBJ::Material* material = mesh->pMaterial;
		const std::string colorMap = material->colorMapFilename;

		GLState state;

		state.material.ambient = glm::vec3(*reinterpret_cast<const glm::vec3*>(material->ambient));
		state.material.diffuse = glm::vec3(*reinterpret_cast<const glm::vec3*>(material->diffuse));
		state.material.specular = glm::vec3(*reinterpret_cast<const glm::vec3*>(material->specular));
		state.material.shininess = material->shininess;
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), this->lightRotation, glm::vec3(0.0f, 1.0f, -1.0f));
		state.movingLight.position = rotation * state.movingLight.position;

		this->shaderProgram->updateMaterialUniforms(state);
		this->shaderProgram->updateLightUniforms(state);

		// reference for the GPU of which texture object to use. Default to 0;
		GLuint texture = 0;

		// If we find a texture object with the right name, then tell OpenGL to use it
		auto it = this->textureObjects.find(colorMap);
		if (it != this->textureObjects.end())
		{
			texture = it->second;
			glUniform1i(this->uNoTexture, GL_FALSE);
		}
		else
		{
			glUniform1i(this->uNoTexture, GL_TRUE);
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

	glDisableVertexAttribArray(vertexLocation);
	//glDisableVertexAttribArray(texCoordsLocation);
	glDisableVertexAttribArray(vertexLocation);
}

glm::mat4 const ContainerOBJ::getModelMatrix() const
{
	return this->modelMatrix;
}

void ContainerOBJ::setShaderProgram(const std::shared_ptr<ShaderProgram> shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

void ContainerOBJ::toggleRotation()
{
	if (this->runRotation)
	{
		this->modelMatrix = glm::mat4(); // set to identity
	}

	this->runRotation = !this->runRotation;
}

void ContainerOBJ::averageVertexNormals()
{
	float* VBOstart = static_cast<float*>(const_cast<ModelOBJ::Vertex*>(this->model->getVertexBuffer())->position);

	std::unordered_map<float, std::unordered_map<float, std::unordered_map<float, std::vector<float*>>>> box;

	printf("Start storing positions...\n");

	model->getMesh(0).startIndex;
	int numVertices = this->model->getNumberOfVertices();
	float* position = VBOstart;
	float x, y, z;
	for (int i = 0; i < numVertices; ++i)
	{
		x = position[0];
		y = position[1];
		z = position[2];
		box[x][y][z].push_back(position);
		position += 15;	// go forward 15 floats in memory (since ModelOBJ::Vertex contains 15 floats)
	}


	printf("Stored!   (positions in place in vector)\n");

	for (auto& xMap : box)
	{
		for (auto& yMap : xMap.second)
		{
			for (auto& zMap : yMap.second)
			{
				float xNormal = 0;
				float yNormal = 0;
				float zNormal = 0;

				for (float* vertex : zMap.second)
				{
					xNormal += *(vertex + 6);	// the 6th float is the first float of the ModelOBJ::Vertex normal
					yNormal += *(vertex + 7);	// the 7th float is the second float of the ModelOBJ::Vertex normal
					zNormal += *(vertex + 8);	// the 8th float is the third float of the ModelOBJ::Vertex normal
				}

				auto size = zMap.second.size();
				xNormal /= size;
				yNormal /= size;
				zNormal /= size;

				for (float* vertex : zMap.second)
				{
					*(vertex + 6) = xNormal;	// the 6th float is the first float of the ModelOBJ::Vertex normal
					*(vertex + 7) = yNormal;	// the 7th float is the second float of the ModelOBJ::Vertex normal
					*(vertex + 8) = zNormal;	// the 8th float is the third float of the ModelOBJ::Vertex normal
				}
			}
		}
	}
}  // end averageVertexNormals()