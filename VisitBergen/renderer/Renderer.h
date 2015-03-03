#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "..\processManager\Process.h"
#include "..\eventManager\EventListener.h"

/* test with mesh */
#include "..\assets\temp\ContainerOBJ.h"

class Renderer :
	public Process,
	public EventListener
{
public:
	Renderer();
	~Renderer();

	virtual void init(void);
	virtual void update(unsigned long deltaMs);
	virtual void handleEvent(const std::shared_ptr<Event>& event);

	void display();

	static std::shared_ptr<Renderer> instance;
	static void setInstance(std::shared_ptr<Renderer> renderer);
	static void displayCall();

	GLuint const getShaderProgram() const;

private:
	GLuint shaderProgram;
	GLint uModelMatrix;
	GLint uModelViewMatrix;
	GLint uProjectionMatrix;
	GLint uTextureSampler;

	glm::mat4 modelMatrix;
	glm::mat4 modelViewMatrix;
	glm::mat4 projectionMatrix;

	bool initShaders();

	/* test with mesh */
	std::shared_ptr<ContainerOBJ> car;

};

