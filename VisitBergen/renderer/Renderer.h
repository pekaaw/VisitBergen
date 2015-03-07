#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "..\processManager\Process.h"
#include "..\eventManager\EventListener.h"

/* test with mesh */
#include "..\assets\temp\ContainerOBJ.h"
#include "..\util\LightSpecifictions.h"

class Renderer :
	public Process,
	public EventListener
{
public:
	enum ProjectionMode
	{
		PERSPECTIVE = 0,
		ORTHOGRAPHIC,
		NumberOfProjectionModes
	};

	Renderer();
	~Renderer();

	virtual void init(void);
	virtual void update(unsigned long deltaMs);
	virtual void handleEvent(const std::shared_ptr<Event>& event_);

	void display();

	static const std::shared_ptr<Renderer> getInstance(void) ;

	GLuint const getShaderProgram() const;
	int const getWindowWidth() const;
	int const getWindowHeight() const;

protected:
	virtual void onAbort();

private:
	GLuint shaderProgram;
	GLint uModelMatrix;
	GLint uViewMatrix;
	GLint uProjectionMatrix;
	GLint uNormalMatrix;
	GLint uTextureSampler;
	GLint uCameraPosition;

	DirectionalLight dirLight;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat3 normalMatrix;

	glm::vec3 cameraPosition;
	glm::vec3 cameraCenter;
	glm::vec3 cameraUpVector;


	bool usePerspectiveMode;

	int windowWidth;
	int windowHeight;

	bool initShaders();
	void setProjectionMode(ProjectionMode mode = ProjectionMode::PERSPECTIVE);

	/* test with mesh */
	std::shared_ptr<ContainerOBJ> car;

	static std::shared_ptr<Renderer> instance;


};

