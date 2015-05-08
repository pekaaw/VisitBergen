#pragma once

#include <thread>
#include <atomic>
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
	std::shared_ptr<ShaderProgram> shaderProgram;

	std::shared_ptr<ModelOBJ> model;
	const char* modelPath;
	const char* modelFormat;
	std::thread importer;
	std::atomic<bool> importingModel = true;

	static void loadObjModel(std::shared_ptr<ModelOBJ>&, const char* modelPath, std::atomic<bool>& isComputing);
	void loadToGPU();
	void drawComponent();

	void loadModelTest();

public:
	explicit MeshComponent();
	static const char* componentName;

	bool preInit(tinyxml2::XMLElement*);
	void init();
	bool dependancyInjection();
	const char* getComponentName();

	void draw();
	void(MeshComponent::*callDrawMethod)();
};

