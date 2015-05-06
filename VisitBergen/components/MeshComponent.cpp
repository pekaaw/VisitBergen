#include <stdlib.h>
#include "MeshComponent.h"

const char* MeshComponent::componentName = "MeshComponent";

MeshComponent::MeshComponent()
{
	this->callDrawMethod = &MeshComponent::loadToGPU;
}

void MeshComponent::init() 
{
	char buffer[33];
	_itoa_s(this->owner->getActorID(), buffer, 10);
	std::string name = buffer;
	name += MeshComponent::componentName;

	Locator::getRenderer()->addRenderable(name, std::shared_ptr<RenderableInterface>(this));
}

void MeshComponent::loadToGPU()
{
	// If the model has not yet been imported, don't do anything
	//printf("ImporingModel: %b\n", this->importingModel);
	if (this->importingModel)
	{
		int i = this->model->getNumberOfVertices();
		return;
	}

	// TODO: Load data to GPU!
	printf("Hei! No kan vi printe til GPU =D\n");

	this->callDrawMethod = &MeshComponent::drawComponent;
}

void MeshComponent::drawComponent()
{
	std::shared_ptr<Renderer> renderer = Locator::getRenderer();
}

void MeshComponent::loadObjModel(std::shared_ptr<ModelOBJ>& model, const char* modelPath, std::atomic<bool>& isComputing)
{
	std::string path(modelPath);
	printf("Starting to import %s\n", path.c_str());
	model->import(path.c_str());
	printf("Finished loading the model from %s\n", path.c_str());
	isComputing = false;
}

void MeshComponent::loadModelTest()
{
	printf("modelPath: %s\n", this->modelPath);
	this->model->import(this->modelPath);
	this->importingModel = false;
}

bool MeshComponent::preInit(tinyxml2::XMLElement* data)
{
	tinyxml2::XMLElement* modelData = data->FirstChildElement("Model");
	if (!modelData)
	{
		printf("MeshComponent has no Model element!\n");
		return false;
	}
	else
	{
		const char* path = modelData->GetText();
		if (path == nullptr)
		{
			printf("MeshComponent has no path to model!\n");
			return false;
		}
		this->modelPath = path;

		if (modelData->Attribute("format", "obj"))
		{
			this->modelFormat = "obj";
			this->model = std::make_shared<ModelOBJ>();
			importer = std::thread([=]{loadModelTest(); });
			//importer = std::thread(&MeshComponent::loadObjModel, this->model, this->modelPath, this->importingModel);
		}
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