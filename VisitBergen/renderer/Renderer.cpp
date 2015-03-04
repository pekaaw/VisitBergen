#include <string>
#include <vector>
#include <assert.h>

#include "../util/includeGL.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Renderer.h"
#include "../util/fileutil.h"

Renderer::Renderer() :
	shaderProgram(0),
	uModelMatrix(-1),
	uModelViewMatrix(-1),
	uProjectionMatrix(-1),
	uTextureSampler(-1)
{
}

Renderer::~Renderer()
{
}

void Renderer::init(void)
{
	this->Process::init();

	this->modelMatrix = glm::mat4();
	this->modelViewMatrix = glm::mat4();
	this->projectionMatrix = glm::mat4();

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

	glUseProgram(0);

	// Set the clearcolor for the gl context
	glClearColor(0.0f, 0.5f, 0.7f, 0.0f);
}

void Renderer::update(unsigned long deltaMs)
{
	this->display();
	return;
}

void Renderer::handleEvent(const std::shared_ptr<Event>& event)
{
	return;
}

void Renderer::display()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable face culling
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// enable the shader program
	assert(this->shaderProgram != 0);
	glUseProgram(this->shaderProgram);

	glUniformMatrix4fv(this->uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(this->modelViewMatrix));
	glUniformMatrix4fv(this->uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));

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
void Renderer::setInstance(std::shared_ptr<Renderer> renderer)
{
	instance = renderer;
}

void Renderer::displayCall()
{
	std::printf("Hello from Renderer::displayCall()\n");
	instance->display();
}

GLuint const Renderer::getShaderProgram() const
{
	return this->shaderProgram;
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
	this->uModelViewMatrix = glGetUniformLocation(this->shaderProgram, "ModelViewMatrix");
	this->uProjectionMatrix = glGetUniformLocation(this->shaderProgram, "ProjectionMatrix");
	this->uTextureSampler = glGetUniformLocation(this->shaderProgram, "TextureSampler");

	assert(this->uModelMatrix != -1 && this->uModelViewMatrix != -1 && this->uProjectionMatrix != -1 && this->uTextureSampler != -1);

	glUniformMatrix4fv(this->uModelMatrix, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
	glUniformMatrix4fv(this->uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(this->modelViewMatrix));
	glUniformMatrix4fv(this->uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));

	// Initialization finished successfully!
	return true;
}