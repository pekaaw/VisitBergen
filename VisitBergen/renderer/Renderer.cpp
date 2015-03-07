#include <string>
#include <vector>
#include <assert.h>

#include "..\util\includeGL.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "Renderer.h"
#include "..\util\fileutil.h"
#include "..\events\EventCameraTransform.h"
#include "..\events\EventToggleProjectionMode.h"
#include "..\events\EventCameraZoom.h"

Renderer::Renderer() :
	shaderProgram(0),
	uModelMatrix(-1),
	uViewMatrix(-1),
	uProjectionMatrix(-1),
	uTextureSampler(-1),
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

	this->modelMatrix = glm::mat4();
	this->viewMatrix = glm::mat4();
	this->projectionMatrix = glm::mat4();

	this->cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	this->cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	this->cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 target = glm::vec3(0.0f);
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	this->viewMatrix *= glm::lookAt(this->cameraPosition, this->cameraCenter, this->cameraUpVector);

	initShaders();
	/*todo: change name to initShaderProgram*/

	// enable the shader program
	assert(this->shaderProgram != 0);
	glUseProgram(this->shaderProgram);

	if (!this->car)
	{
		car = std::make_shared<ContainerOBJ>();
	}

	car->init("assets\\car.obj");
	car->init("assets\\cube\\cube.obj");

	glUseProgram(0);

	// Set the clearcolor for the gl context
	glClearColor(0.0f, 0.5f, 0.7f, 0.0f);

	setProjectionMode(PERSPECTIVE);

	// setup for Directional Light
	glUniform3fv(this->dirLight.uDirection, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, -1.0f)));
	glUniform3fv(this->dirLight.uAmbientColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
	glUniform3fv(this->dirLight.uDiffuseColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
	glUniform3fv(this->dirLight.uSpecularColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 1.0f)));
	glUniform1f(this->dirLight.uAmbientIntensity, 0.2f);
	glUniform1f(this->dirLight.uDiffuseIntensity, 0.3f);
	glUniform1f(this->dirLight.uSpecularIntensity, 0.2f);
	
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
		this->viewMatrix *= transformEvent->getTransform();
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

	if (std::shared_ptr<EventCameraZoom> zoomEvent = std::dynamic_pointer_cast<EventCameraZoom>(event_))
	{
		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(zoomEvent->getZoom()));
		this->viewMatrix *= scale;
		printf("Renderer::handleEvent(EventCameraZoom(%f))\n", zoomEvent->getZoom());
	}
}

void Renderer::display()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable face culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// enable the shader program
	assert(this->shaderProgram != 0);
	glUseProgram(this->shaderProgram);

	glUniformMatrix4fv(this->uViewMatrix, 1, GL_FALSE, glm::value_ptr(this->viewMatrix));
	glUniformMatrix4fv(this->uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
	this->normalMatrix = glm::inverseTranspose(glm::mat3(this->viewMatrix));
	glUniformMatrix3fv(this->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->normalMatrix));
	glUniformMatrix3fv(this->uCameraPosition, 1, GL_FALSE, glm::value_ptr(this->cameraPosition));

	// TODO: Do amazing stuff by rendering!
	if (this->car)
	{
		glUniform1i(this->uTextureSampler, 0);
		glUniformMatrix4fv(this->uModelMatrix, 1, GL_FALSE, glm::value_ptr(this->car->getModelMatrix()));
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
	return this->shaderProgram;
}

int const Renderer::getWindowWidth() const
{
	return this->windowWidth;
}

int const Renderer::getWindowHeight() const
{
	return this->windowHeight;
}

bool Renderer::initShaders()
{
	// Delete old shaderProgram if it exists
	if (this->shaderProgram != 0)
	{
		glDeleteProgram(this->shaderProgram);
	}

	// Create new shader program
	this->shaderProgram = glCreateProgram();
	if (this->shaderProgram == 0)
	{
		printf("Error: Cannot create shader program.\n");
		return false;
	}

	// Create shader objects
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (vertShader == 0 || fragShader == 0)
	{
		printf("Error: Unable to create shader objects.");
		return false;
	}

	std::string vertexShaderText = readTextFile("shader//vertexshader.glsl");
	const char* vertexShaderSource = vertexShaderText.c_str();
	int vsLength = static_cast<int>(vertexShaderText.length());
	if (vsLength == 0)
	{
		printf("Error: Nothing received from file with vertex shader.\n");
		return false;
	}

	std::string fragmentShaderText = readTextFile("shader//fragmentshader.glsl");
	const char* fragmentShaderSource = fragmentShaderText.c_str();
	int fsLength = static_cast<int>(fragmentShaderText.length());
	if (fsLength == 0)
	{
		printf("Error: Nothing received from file with fragment shader.\n");
		return false;
	}

	glShaderSource(vertShader, 1, &vertexShaderSource, &vsLength);
	glShaderSource(fragShader, 1, &fragmentShaderSource, &fsLength);

	glCompileShader(vertShader);
	glCompileShader(fragShader);

	GLint success = 0;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Get length of InfoLog, create vector with length and put errorLog into vector
		GLint infoLength = 0;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetShaderInfoLog(vertShader, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error compiling vertex shader:\n %s \n", errorInfoLog.c_str());

		// Don't leak shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return false;
	}

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		// Get length of InfoLog, create vector with length and put errorLog into vector
		GLint infoLength = 0;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetShaderInfoLog(fragShader, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error compiling fragment shader:\n %s \n", errorInfoLog.c_str());

		// Don't leak shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return false;
	}

	// Attach shaders to program and link it
	glAttachShader(this->shaderProgram, vertShader);
	glAttachShader(this->shaderProgram, fragShader);
	glLinkProgram(this->shaderProgram);

	// Check for linking error
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetProgramInfoLog(this->shaderProgram, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error compiling fragment shader: \n %s \n", errorInfoLog.c_str());

		// Don't leak program or shaders
		glDeleteProgram(this->shaderProgram);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return false;
	}

	// Detach shaders after successful linking
	glDetachShader(this->shaderProgram, vertShader);
	glDetachShader(this->shaderProgram, fragShader);

	// Now we don't need the shaders anymore
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Validate program
	glValidateProgram(this->shaderProgram);

	// Check for validation error
	glGetProgramiv(this->shaderProgram, GL_VALIDATE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
		std::vector<GLchar> errorLog(infoLength);
		glGetProgramInfoLog(this->shaderProgram, infoLength, &infoLength, errorLog.data());

		// Construct string and output error
		std::string errorInfoLog(errorLog.begin(), errorLog.end());
		printf("Error: Invalid shaderprogram: \n %s \n", errorInfoLog.c_str());
	}

	// Get uniform locations
	this->uModelMatrix = glGetUniformLocation(this->shaderProgram, "ModelMatrix");
	this->uViewMatrix = glGetUniformLocation(this->shaderProgram, "ViewMatrix");
	this->uProjectionMatrix = glGetUniformLocation(this->shaderProgram, "ProjectionMatrix");
	this->uNormalMatrix = glGetUniformLocation(this->shaderProgram, "NormalMatrix");
	this->uCameraPosition = glGetUniformLocation(this->shaderProgram, "CameraPosition");
	this->uTextureSampler = glGetUniformLocation(this->shaderProgram, "TextureSampler");

	// get uniform locations for light
	this->dirLight.uDirection = glGetUniformLocation(this->shaderProgram, "DirectionalLightDirection");
	this->dirLight.uAmbientColor = glGetUniformLocation(this->shaderProgram, "DirectionalLightAmbientColor");
	this->dirLight.uDiffuseColor = glGetUniformLocation(this->shaderProgram, "DirectionalLightDiffuseColor");
	this->dirLight.uSpecularColor = glGetUniformLocation(this->shaderProgram, "DirectionalLightSpecularColor");
	this->dirLight.uAmbientIntensity = glGetUniformLocation(this->shaderProgram, "DirectionalLightAmbientIntensity");
	this->dirLight.uDiffuseIntensity = glGetUniformLocation(this->shaderProgram, "DirectionalLightDiffuseIntentsity");
	this->dirLight.uSpecularIntensity = glGetUniformLocation(this->shaderProgram, "DirectionalLightSpecularIntensity");

	assert(this->uModelMatrix != -1 && this->uViewMatrix != -1 && this->uProjectionMatrix != -1 && this->uTextureSampler != -1);

	glUniformMatrix4fv(this->uModelMatrix, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
	glUniformMatrix4fv(this->uViewMatrix, 1, GL_FALSE, glm::value_ptr(this->viewMatrix));
	glUniformMatrix4fv(this->uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
	glUniformMatrix3fv(this->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(this->normalMatrix));

	// Initialization finished successfully!
	return true;
}

void Renderer::setProjectionMode(Renderer::ProjectionMode mode)
{
	switch (mode)
	{
	default:
	case PERSPECTIVE:
		this->projectionMatrix = glm::perspective(45.0f, float(this->windowWidth) / this->windowWidth, 0.1f, 100.0f);
		break;
	case ORTHOGRAPHIC:
		float height = float(this->windowHeight) / float(this->windowWidth) * 0.5f;
		this->projectionMatrix = glm::ortho(-0.5f, 0.5f, -height, height, 0.1f, 100.0f);
		break;
	}
}

void Renderer::onAbort()
{
	// Release the car object (call destructor if no one else is using it
	this->car.reset();

	// delete the program
	if (glIsProgram(this->shaderProgram))
	{
		glDeleteProgram(this->shaderProgram);
	}

}