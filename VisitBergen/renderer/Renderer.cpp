#include <string>
#include <vector>
#include <assert.h>

#include "..\util\includeGL.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "Renderer.h"
//#include "..\util\fileutil.h"
#include "..\events\EventCameraTransform.h"
#include "..\events\EventToggleProjectionMode.h"
#include "..\events\EventToggleModelRotation.h"

Renderer::Renderer() :
	//shaderProgram(0),
	//uModelMatrix(-1),
	//uViewMatrix(-1),
	//uProjectionMatrix(-1),
	//uTextureSampler(-1),
	usePerspectiveMode(true),
	windowWidth(800),
	windowHeight(600),
	lightRotation(0.0f)
{
}

Renderer::~Renderer()
{
	this->renderables.clear();
}

void Renderer::init(void)
{
	this->Process::init();

	if (!this->camera)
	{
		this->camera = std::make_shared<Camera>();
	}

	glUseProgram(0);

	// Set the clearcolor for the gl context
	glClearColor(0.0f, 0.5f, 0.7f, 0.0f);

	setProjectionMode(PERSPECTIVE);

}

void Renderer::update(unsigned long deltaMs)
{
	this->EventListener::update();
	this->display();
	return;
}

void Renderer::handleEvent(const std::shared_ptr<Event>& event_)
{
	if (std::shared_ptr<EventCameraTransform> transformEvent = std::dynamic_pointer_cast<EventCameraTransform>(event_))
	{
		glm::mat4 transform = this->camera->transform(transformEvent->getTransform());
		glm::mat4 view = this->camera->getViewMatrix();
		view[0][2] += transform[0][0];	// add x from vector to view direction
		view[2][2] += transform[0][2];	// add z from vector to view direction
		

		for (auto it : this->shaderPrograms)
		{
			it.second->updateViewUniform(view);
		}
	}

	else if (std::shared_ptr<EventToggleProjectionMode> projectionEvent = std::dynamic_pointer_cast<EventToggleProjectionMode>(event_))
	{
		this->usePerspectiveMode = !this->usePerspectiveMode;

		if (this->usePerspectiveMode)
		{
			setProjectionMode(PERSPECTIVE);
		}
		else
		{
			setProjectionMode(ORTHOGRAPHIC);
		}
	}

	else if (std::shared_ptr<EventToggleModelRotation> toggleEvent = std::dynamic_pointer_cast<EventToggleModelRotation>(event_))
	{
		// Call some code to rotate a model
		std::shared_ptr<Actor> teddy2 = Locator::getActorManager()->getActor("Teddy2");
		if (teddy2)
		{
			std::shared_ptr<MeshComponent> component = std::static_pointer_cast<MeshComponent>(teddy2->getComponent("MeshComponent"));
			if (component)
				component->toggleRotation();
		}		
		
		// Call some code to rotate a model
		std::shared_ptr<Actor> car = Locator::getActorManager()->getActor("Car");
		if (car)
		{
			std::shared_ptr<MeshComponent> component = std::static_pointer_cast<MeshComponent>(car->getComponent("MeshComponent"));
			if (component)
				component->toggleRotation();
		}
	}
}

void Renderer::display()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable face culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// enable the shader program
	//assert(this->shaderProgram != 0);
	glUseProgram(this->shaderPrograms["phongLighting"]->shaderProgram);

	GLState frameState;
	frameState.modelMatrix = glm::mat4();
	frameState.viewMatrix = this->camera->getViewMatrix();
	frameState.projectionMatrix = this->camera->getProjectionMatrix();

	this->lightRotation += 0.01;
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), this->lightRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	frameState.movingLight.position = rotation * frameState.movingLight.position;

	// Update modelViewProjectionMatrices and lights for all the shaderprograms
	for (auto it : this->shaderPrograms)
	{
		glUseProgram(it.second->shaderProgram);
		it.second->updateLightUniforms(frameState);
		it.second->updateModelViewProjectionUniforms(frameState);
		glUniform1f(it.second->uniformIDs["WaveTime"], this->lightRotation);
	}

	// Call all renderable objects draw method	
	for (auto& it : this->renderables)
	{
		it.second->draw();
	}


	glUseProgram(0);

	// Swap frame buffers (off-screen rendering)
	glutSwapBuffers();
}

std::shared_ptr<Renderer> Renderer::instance = nullptr;
const std::shared_ptr<Renderer> Renderer::getInstance(void)
{
	if (Renderer::instance == nullptr)
	{
		Renderer::instance = std::make_shared<Renderer>();
	}

	return Renderer::instance;
}

std::shared_ptr<Camera> Renderer::getCamera()
{
	if (!this->camera)
	{
		this->camera = std::make_shared<Camera>();
	}

	return this->camera;
}

std::shared_ptr<ShaderProgram> Renderer::getShaderProgram(std::string name)
{
	auto it = this->shaderPrograms.find(name);

	if (it != this->shaderPrograms.end())
	{
		// We found the shaderProgram and will return it
		return it->second;
	}

	// We did not find it!
	if (name.length() > 0)
	{
		std::shared_ptr<ShaderProgram> program = this->shaderFactory.makeShaderProgram(name);
		if (program)
		{
			this->shaderPrograms[name] = program;
			return program;
		}
	}

	// ShaderProgram not found and could not be made. Make sure we have a default and return that one.

	if (!this->shaderPrograms.empty())
	{
		return this->shaderPrograms.begin()->second;
	}
	else
	{
		assert(!this->shaderPrograms.empty()); 
		// We should not get here. If that is the case, we have tried to access
		// a shader without loading one. A solution is to load a shader we will
		// be using,or assuming we will use, so that it can be used as default.
		return nullptr;
	}
}

void Renderer::reshape(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	this->setProjectionMode(this->currentProjectionMode);
}

int const Renderer::getWindowWidth() const
{
	return this->windowWidth;
}

int const Renderer::getWindowHeight() const
{
	return this->windowHeight;
}

void Renderer::setProjectionMode(Renderer::ProjectionMode mode)
{
	glm::mat4 projectionMatrix;

	switch (mode)
	{
	default:
	case PERSPECTIVE:
		this->currentProjectionMode = mode;
		this->camera->setProjectionMatrix(glm::perspective(45.0f, float(this->windowWidth) / this->windowHeight, 0.1f, 10000.0f));
		projectionMatrix = this->camera->getProjectionMatrix();
		break;
	case ORTHOGRAPHIC:
		this->currentProjectionMode = mode;
		float aspect = float(this->windowHeight) / float(this->windowWidth);
		float halfWidth = 0.5;
		this->camera->setProjectionMatrix(glm::ortho(-halfWidth, halfWidth, -halfWidth * aspect, halfWidth * aspect, 0.1f, 100.0f));
		projectionMatrix = this->camera->getProjectionMatrix();
		break;
	}

	// Update projection uniform matrix for all shaderprograms
	for (auto it : this->shaderPrograms)
	{
		it.second->updateProjectionUniform(projectionMatrix);
	}
}

void Renderer::onAbort()
{
}


void Renderer::addRenderable(std::string name, std::shared_ptr<RenderableInterface> objectPtr)
{
	auto it = this->renderables.find(name);

	if (it == this->renderables.end())
	{
		this->renderables[name] = objectPtr;
		return;
	}

	printf("The renderable object has already been registered with the renderer!\n");
}

void Renderer::removeRenderable(std::string name)
{
	auto it = this->renderables.find(name);

	if (it != this->renderables.end())
	{
		this->renderables.erase(it);
		printf("The renderable \"%s\" was detached from the render loop.\n", name);
	}
}
