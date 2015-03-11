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

Renderer::Renderer() :
	//shaderProgram(0),
	//uModelMatrix(-1),
	//uViewMatrix(-1),
	//uProjectionMatrix(-1),
	//uTextureSampler(-1),
	usePerspectiveMode(true),
	windowWidth(800),
	windowHeight(600)
{
}

Renderer::~Renderer()
{
}

void Renderer::init(void)
{
	this->Process::init();

	this->camera = std::make_shared<Camera>();

	// Add a shaderProgram to use
	std::shared_ptr<ShaderProgram> phongLightingProgram = this->shaderFactory.makeShaderProgram("phongLighting");
	if (!phongLightingProgram)
	{
		// Initialization failed, notify ProcessManager that process failed...
		//Process::fail();
		return;
	}

	this->shaderPrograms["phongLighting"] = phongLightingProgram;

	if (!this->car)
	{
		car = std::make_shared<ContainerOBJ>();
		car->setShaderProgram(this->shaderPrograms["phongLighting"]);
		GLState state;
		state.viewMatrix = this->camera->getViewMatrix();
		this->shaderPrograms["phongLighting"]->updateAllUniforms(state);
	}

	if (!car->init("assets\\car.obj"))
	//if (!car->init("assets\\capsule\\capsule.obj"))
	//if (!car->init("assets\\cube\\cube.obj"))
	{
		printf("Car not loaded.");
	}

	//glUseProgram(0);

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
		glm::mat4 viewMatrix = this->camera->transform(transformEvent->getTransform());
		this->shaderPrograms["phongLighting"]->updateViewUniform(viewMatrix);
		//printf("Renderer received CameraTransform\n");
	}

	if (std::shared_ptr<EventToggleProjectionMode> projectionEvent = std::dynamic_pointer_cast<EventToggleProjectionMode>(event_))
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

	this->shaderPrograms["phongLighting"]->updateModelViewProjectionUniforms(frameState);


	// TODO: Do amazing stuff by rendering!
	if (this->car)
	{
		this->car->draw();
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

GLuint const Renderer::getShaderProgram() const
{
	//return this->shaderProgram;
	return 0;
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
	switch (mode)
	{
	default:
	case PERSPECTIVE:
		this->camera->setProjectionMatrix(glm::perspective(45.0f, float(this->windowWidth) / this->windowWidth, 0.1f, 100.0f));
		this->shaderPrograms["phongLighting"]->updateProjectionUniform(this->camera->getProjectionMatrix());
		//this->projectionMatrix = glm::perspective(45.0f, float(this->windowWidth) / this->windowWidth, 0.1f, 100.0f);
		break;
	case ORTHOGRAPHIC:
		float height = float(this->windowHeight) / float(this->windowWidth) * 0.5f;
		this->camera->setProjectionMatrix(glm::ortho(-0.5f, 0.5f, -height, height, 0.1f, 100.0f));
		this->shaderPrograms["phongLighting"]->updateProjectionUniform(this->camera->getProjectionMatrix());
		//this->projectionMatrix = glm::ortho(-0.5f, 0.5f, -height, height, 0.1f, 100.0f);
		break;
	}
}

void Renderer::onAbort()
{
	// Release the car object (call destructor if no one else is using it
	this->car.reset();

}