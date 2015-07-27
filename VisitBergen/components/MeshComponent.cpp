#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "MeshComponent.h"
#include "..\thirdParty\lodepng.h"

const char* MeshComponent::componentName = "MeshComponent";

MeshComponent::MeshComponent() :
doNormalization(true),
doNormalAveraging(false),
doRotation(false),
rotation(0.0f)
{
	this->callDrawMethod = &MeshComponent::loadToGPU;
	this->shaderProgram = nullptr;
}

MeshComponent::~MeshComponent()
{
	delete [] this->modelPath;

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

	// Delete all textures
	for (auto it : this->textureLocations)
	{
		GLuint texture = it.second;
		if (glIsTexture(texture))
		{
			glDeleteTextures(1, &texture);
		}
	}

	// Clear texture map
	this->textureLocations.clear();

	// release the model
	this->model.reset();
}

void MeshComponent::init() 
{
	// Add this MeshComponent as a renderable in the renderer, with ID like
	// XMeshComponent where X is the ActorID.
	char buffer[33];
	_itoa_s(this->owner->getActorID(), buffer, 10);
	std::string name = buffer;
	name += MeshComponent::componentName;

	//TODO: This does not work!!!!!!
	//Locator::getRenderer()->addRenderable(name, std::shared_ptr<RenderableInterface>(this));
	Locator::getRenderer()->addRenderable(name, shared_from_this());
}

void MeshComponent::setRotation(bool state)
{
	this->doRotation = state;
}

void MeshComponent::toggleRotation()
{
	this->doRotation = !this->doRotation;
}

void MeshComponent::loadToGPU()
{
	// If the model has not yet been imported, don't do anything
	//printf("ImporingModel: %b\n", this->importingModel);
	if (this->importingModel)
	{
		return;
	}

	// Make sure we have a shaderProgram before we try to load to the GPU
	if (this->shaderProgram == nullptr)
	{
		this->shaderProgram = Locator::getRenderer()->getShaderProgram(this->shaderName);
		if (this->shaderProgram == nullptr)
		{
			return;
		}
	}

	this->importer.join();

	printf("\a %s loaded into memory. Starting normalization.\n", this->modelPath);

	if (this->doNormalization)
	{
		this->model->normalize();
	}

	if (this->doNormalAveraging)
	{
		this->averageVertexNormals(
			static_cast<float*>(const_cast<ModelOBJ::Vertex*>(this->model->getVertexBuffer())->position),
			this->model->getNumberOfVertices());
	}

	printf("\a %s normalized. Start loading to GPU.\n", this->modelPath);

	GLuint program = this->shaderProgram->shaderProgram;
	glUseProgram(program);

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

	//this->uNoTextureBool = glGetUniformLocation(program, "NoTexture");

	// Load texture for all materials
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

			printf("Loaded %s\n", textureFileName->c_str());

			if (result != 0)
			{
				printf("Error: Texture \'%s\' was not successfully loaded for \'%s\'.\n", textureFileName->c_str(), this->modelPath);
				continue;
			}

			GLuint texture = 0;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			// Load image data to texture location bound to GL_TEXTURE_2D
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_SRGB_ALPHA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image.data());

			// add texture reference to texture map
			this->textureLocations[*textureFileName] = texture;

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// unbind texture by setting it to 0
			glBindTexture(GL_TEXTURE_2D, 0);

			// Data loaded to GPU, can free it from HEAP
			image.clear();
			png.clear();
		}
	}

	// TODO: Load data to GPU!
	printf("\a Done loading \'%s\' to GPU =D\n", this->modelPath);

	this->callDrawMethod = &MeshComponent::drawComponent;
}

void MeshComponent::drawComponent()
{
	glUseProgram(this->shaderProgram->shaderProgram);

	glm::mat4 modelMatrix = this->transform->getTransform();
	if (this->doRotation)
	{
		this->rotation += 0.05f;
		modelMatrix = glm::rotate(modelMatrix, this->rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	this->shaderProgram->updateModelUniform(modelMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);

	GLint vertexLocation = shaderProgram->attribLocations["vertex"];
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(0));

	GLint texCoordLocation = this->shaderProgram->attribLocations["tex_coords"];
	glEnableVertexAttribArray(texCoordLocation);
	glVertexAttribPointer(texCoordLocation,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(3 * sizeof(float)));

	GLint normalLocation = this->shaderProgram->attribLocations["normal"];
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelOBJ::Vertex),
		reinterpret_cast<const GLvoid*>(5 * sizeof(float)));

	// now draw each mesh
	int numMeshes = this->model->getNumberOfMeshes();
	for (int m = 0; m < numMeshes; ++m)
	{
		const ModelOBJ::Mesh* mesh = &this->model->getMesh(m);
		const ModelOBJ::Material* material = mesh->pMaterial;
		const std::string colorMap = material->colorMapFilename;

		GLState state;
		state.material.ambient = glm::vec3(*reinterpret_cast<const glm::vec3*>(material->ambient));
		state.material.diffuse = glm::vec3(*reinterpret_cast<const glm::vec3*>(material->diffuse));
		state.material.specular = glm::vec3(*reinterpret_cast<const glm::vec3*>(material->specular));
		state.material.shininess = material->shininess;

		this->shaderProgram->updateMaterialUniforms(state);

		GLuint texture = 0;
		
		auto it = this->textureLocations.find(colorMap);
		if (it != this->textureLocations.end())
		{
			texture = it->second;
			glUniform1i(this->shaderProgram->uniformIDs["NoTexture"], GL_FALSE);
			glUniform1i(this->shaderProgram->uniformIDs["TextureSampler"], 0); // use GL_TEXTURE0
		}
		else
		{
			glUniform1i(this->shaderProgram->uniformIDs["NoTexture"], GL_TRUE);
		}

		// set active texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// select part of mesh to render
		int startIndex = mesh->startIndex;
		int numIndices = mesh->triangleCount * 3;

		// Ask OpenGL to draw!
		glDrawElements(GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			reinterpret_cast<const GLvoid*>(startIndex * sizeof(GL_UNSIGNED_INT)));

		glBindTexture(GL_TEXTURE_2D, 0);
	} // end for each mesh


	glDisableVertexAttribArray(normalLocation);
	glDisableVertexAttribArray(texCoordLocation);
	glDisableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//printf("DRAW!");
	return;
}

void MeshComponent::loadObjModel()
{
	printf("modelPath: %s\n", this->modelPath);
	this->model->import(this->modelPath);
	//this->shaderProgram = Locator::getRenderer()->getShaderProgram(this->shaderName);
	this->importingModel = false;
}

bool MeshComponent::preInit(tinyxml2::XMLElement* data)
{
	// LOAD Model TAG
	tinyxml2::XMLElement* modelData = data->FirstChildElement("Model");
	if (!modelData)
	{
		printf("MeshComponent has no Model element!\n");
		return false;
	}

	const char* path = modelData->GetText();
	if (path == nullptr)
	{
		printf("MeshComponent has no path to model!\n");
		return false;
	}

	// store a copy of the path

	const size_t length = std::strlen(path);
	char* pathCopy = new char[length + 1];
	strncpy_s(pathCopy, length+1, path, length);
	pathCopy[length] = '\0';

	this->modelPath = &pathCopy[0];

	//this->modelPath = (std::string(path) + std::string('\0')).c_str();

	if (modelData->Attribute("format", "obj"))
	{
		this->modelFormat = "obj";
		this->model = std::make_shared<ModelOBJ>();
		this->importer = std::thread([=]{loadObjModel(); });
	}

	// LOAD ShaderProgram TAG
	tinyxml2::XMLElement* shaderProgramData = data->FirstChildElement("ShaderProgram");
	if (shaderProgramData != nullptr)
	{
		const char* shaderNamePtr = shaderProgramData->GetText();
		if (shaderNamePtr != nullptr)
		{
			// store shaderName for later retrieval
			this->shaderName = shaderNamePtr;
		}
	}

	// Do normalization?
	tinyxml2::XMLElement* normalizationData = data->FirstChildElement("Normalize");
	if (normalizationData != nullptr)
	{
		normalizationData->QueryBoolText(&this->doNormalization);
	}

	// Do averaging of normals?
	tinyxml2::XMLElement* averagingData = data->FirstChildElement("AverageNormals");
	if (averagingData != nullptr)
	{
		averagingData->QueryBoolText(&this->doNormalAveraging);
	}

	return true;
}

bool MeshComponent::dependancyInjection()
{
	this->transform = std::dynamic_pointer_cast<TransformComponent>(this->owner->getComponent("TransformComponent"));
	if (this->transform == nullptr)
	{
		return false;
	}

	return true;
}

const char* MeshComponent::getComponentName()
{
	return this->componentName;
}


void MeshComponent::draw()
{
	(this->*callDrawMethod)();
}

void MeshComponent::averageVertexNormals(float* VBO_ptr, int numVertices)
{
	std::unordered_map<float, std::unordered_map<float, std::unordered_map<float, std::vector<float*>>>> box;

	printf("Start storing positions...\n");

	float* position = VBO_ptr;
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
}