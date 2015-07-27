#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "SkyBoxComponent.h"
#include "..\thirdParty\lodepng.h"

const char* SkyBoxComponent::componentName = "SkyBoxComponent";

SkyBoxComponent::SkyBoxComponent() :
isCorrupted(false),
importingModel(true)
{
	this->callDrawMethod = &SkyBoxComponent::loadToGPU;
	this->shaderProgram = nullptr;

}

SkyBoxComponent::~SkyBoxComponent()
{
	delete[] this->modelPath;
	for (int i = 0; i < 6; ++i)
	{
		if (this->imagePath[i])
			delete this->imagePath[i];
	}

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

void SkyBoxComponent::init()
{
	// Add this MeshComponent as a renderable in the renderer, with ID like
	// XMeshComponent where X is the ActorID.
	char buffer[33];
	_itoa_s(this->owner->getActorID(), buffer, 10);
	std::string name = buffer;
	name += SkyBoxComponent::componentName;

	//TODO: This does not work!!!!!!
	//Locator::getRenderer()->addRenderable(name, std::shared_ptr<RenderableInterface>(this));
	Locator::getRenderer()->addRenderable(name, shared_from_this());
}

void SkyBoxComponent::loadToGPU()
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

	this->callDrawMethod = &SkyBoxComponent::drawComponent;


	this->shaderProgram->shaderProgram;
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

	// Load cubemap textures

	
	long cubeMapSides[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};


	glGenTextures(1, &this->textureObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureObject);

	// Load texture for all materials
	for (int i = 0; i < 6; ++i)
	{
		std::vector<unsigned char> png;
		std::vector<unsigned char> image;
		unsigned width, height;

		lodepng::load_file(png, this->imagePath[i]);
		unsigned int result = lodepng::decode(image, width, height, png);

		printf("Loaded %s.\n", this->imagePath[i]);
		if (result != 0)
		{
			printf("Skybox got corrupted due to %s that would not load.", this->imagePath[i]);
			this->isCorrupted = true;
			return;
		}

		glTexImage2D(
			cubeMapSides[i],
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image.data());

		image.clear();
		png.clear();
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	this->cubemapTextureLocation = glGetUniformLocation(this->shaderProgram->shaderProgram, "CubeMapTexture");

	this->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));

	// TODO: Load data to GPU!
	printf("\a Done loading \'%s\' to GPU =D\n", this->modelPath);

}

void SkyBoxComponent::drawComponent()
{
	if (this->isCorrupted)
		return;

	glUseProgram(this->shaderProgram->shaderProgram);
	
	std::shared_ptr<Camera> cam = Locator::getRenderer()->getCamera();

	glm::vec4 tran = glm::inverse(cam->getViewMatrix()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tran.x, tran.y, tran.z));
	glm::mat4 test = transMatrix * this->modelMatrix;
	this->shaderProgram->updateModelUniform(test);

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

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

	//GLint texCoordLocation = this->shaderProgram->attribLocations["tex_coords"];
	//glEnableVertexAttribArray(texCoordLocation);
	//glVertexAttribPointer(texCoordLocation,
	//	2,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(ModelOBJ::Vertex),
	//	reinterpret_cast<const GLvoid*>(3 * sizeof(float)));

	//GLint normalLocation = this->shaderProgram->attribLocations["normal"];
	//glEnableVertexAttribArray(normalLocation);
	//glVertexAttribPointer(normalLocation,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(ModelOBJ::Vertex),
	//	reinterpret_cast<const GLvoid*>(5 * sizeof(float)));

	// now draw each mesh
	int numMeshes = this->model->getNumberOfMeshes();
	for (int m = 0; m < numMeshes; ++m)
	{
		const ModelOBJ::Mesh* mesh = &this->model->getMesh(m);

		// set active texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureObject);

		glUniform1i(this->cubemapTextureLocation, 0);

		// select part of mesh to render
		int startIndex = mesh->startIndex;
		int numIndices = mesh->triangleCount * 3;

		// Ask OpenGL to draw!
		glDrawElements(GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			reinterpret_cast<const GLvoid*>(startIndex * sizeof(GL_UNSIGNED_INT)));

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	} // end for each mesh


	//glDisableVertexAttribArray(normalLocation);
	//glDisableVertexAttribArray(texCoordLocation);
	glDisableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);

	//printf("DRAW!");
	return;
}

void SkyBoxComponent::loadObjModel()
{
	printf("modelPath: %s\n", this->modelPath);
	this->model->import(this->modelPath);
	this->importingModel = false;
}

bool SkyBoxComponent::preInit(tinyxml2::XMLElement* data)
{
	// LOAD Model TAG
	tinyxml2::XMLElement* modelData = data->FirstChildElement("Model");
	if (!modelData)
	{
		printf("SkyBoxComponent has no Model element!\n");
		return false;
	}

	const char* path = modelData->GetText();
	if (path == nullptr)
	{
		printf("SkyBoxComponent has no path to model!\n");
		return false;
	}

	// store a copy of the path

	const size_t length = std::strlen(path);
	char* pathCopy = new char[length + 1];
	strncpy_s(pathCopy, length + 1, path, length);
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

	// LOAD path to skybox images
	tinyxml2::XMLElement* skyBoxSides = data->FirstChildElement("SkyBoxSides");
	if (skyBoxSides == nullptr)
	{
		printf("SkyBoxSides not specified in SkyBoxComponent!\n");
		return false;
	}
	if (skyBoxSides != nullptr)
	{
		const char* names[6] = { "PositiveX", "NegativeX", "PositiveY", "NegativeY", "PositiveZ", "NegativeZ" };
		for (int x = 0; x < 6; ++x)
		{
			tinyxml2::XMLElement* sideData = skyBoxSides->FirstChildElement(names[x]);
			if (sideData == nullptr)
			{
				printf("%s not specified in SkyBoxComponent.\n", names[x]);
				return false;
			}
			else
			{
				const char* path = sideData->Attribute("path");
				if (path == nullptr)
				{
					printf("%s had no path attribute in SkyBoxComponent.\n", names[x]);
					return false;
				}
				this->imagePath[x] = this->getCopyOfConstCharPtr(path);
			}
		}
	}

	return true;
}

bool SkyBoxComponent::dependancyInjection()
{
	return true;
}

const char* SkyBoxComponent::getComponentName()
{
	return this->componentName;
}


void SkyBoxComponent::draw()
{
	(this->*callDrawMethod)();
}

const char* SkyBoxComponent::getCopyOfConstCharPtr(const char* in)
{
	const size_t length = std::strlen(in);
	char* pathCopy = new char[length + 1];
	strncpy_s(pathCopy, length + 1, in, length);
	pathCopy[length] = '\0';

	return &pathCopy[0];
}