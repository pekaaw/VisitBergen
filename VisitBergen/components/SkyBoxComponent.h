#pragma once

#include <thread>
#include <atomic>
#include <string>
#include <tinyxml2.h>
#include "..\thirdParty\model_obj.h"
#include "..\actorManager\Actor.h"
#include "..\actorManager\ActorComponent.h"
#include "TransformComponent.h"
#include "..\shader\ShaderProgram.h"
#include "..\renderer\Renderer.h"
#include "..\interfaces\Locator.h"
#include "..\interfaces\RenderableInterface.h"

class SkyBoxComponent : public ActorComponent, public RenderableInterface, public std::enable_shared_from_this<SkyBoxComponent>
{
private:

	std::shared_ptr<ModelOBJ> model;
	const char* modelPath;
	const char* modelFormat;
	std::thread importer;
	std::atomic<bool> importingModel = true;

	const char* imagePath[6];
	bool isCorrupted;

	// name and pointer to shader for this model
	std::string shaderName;
	std::shared_ptr<ShaderProgram> shaderProgram;

	// buffer object locations for vertices and indices
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	GLuint textureObject;
	GLint cubemapTextureLocation;
	glm::mat4 modelMatrix;

	// map texture name to texture location in OpenGL context
	std::unordered_map<std::string, GLuint> textureLocations;

	void loadToGPU();
	void drawComponent();

	void loadObjModel();

public:
	explicit SkyBoxComponent();
	~SkyBoxComponent();
	static const char* componentName;

	bool preInit(tinyxml2::XMLElement*);
	void init();
	void setRotation(bool state);
	void toggleRotation();
	bool dependancyInjection();
	const char* getComponentName();

	void draw();
	void(SkyBoxComponent::*callDrawMethod)();

	static const char* getCopyOfConstCharPtr(const char* in);
};

