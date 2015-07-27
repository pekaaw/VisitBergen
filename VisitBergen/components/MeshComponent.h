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

class MeshComponent : public ActorComponent, public RenderableInterface, public std::enable_shared_from_this<MeshComponent>
{
private:
	std::shared_ptr<TransformComponent> transform;

	std::shared_ptr<ModelOBJ> model;
	const char* modelPath;
	const char* modelFormat;
	std::thread importer;
	std::atomic<bool> importingModel = true;
	bool doNormalization;
	bool doNormalAveraging;
	bool doRotation;
	float rotation;

	// name and pointer to shader for this model
	std::string shaderName;
	std::shared_ptr<ShaderProgram> shaderProgram;

	// buffer object locations for vertices and indices
	GLuint vertexBufferObject;
	GLuint indexBufferObject;

	// map texture name to texture location in OpenGL context
	std::unordered_map<std::string, GLuint> textureLocations;

	void loadToGPU();
	void drawComponent();

	void loadObjModel();

	static void averageVertexNormals(float* VBO_ptr, int numVertices);

public:
	explicit MeshComponent();
	~MeshComponent();
	static const char* componentName;

	bool preInit(tinyxml2::XMLElement*);
	void init();
	void setRotation(bool state);
	void toggleRotation();
	bool dependancyInjection();
	const char* getComponentName();

	void draw();
	void(MeshComponent::*callDrawMethod)();
};

