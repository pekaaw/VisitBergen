#pragma once

#include <map>
#include <string>
#include <memory>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "..\processManager\Process.h"
#include "..\eventManager\EventListener.h"

/* test with mesh */
#include "..\assets\temp\ContainerOBJ.h"

#include "Camera.h"
#include "..\shader\ShaderFactory.h"
#include "..\components\MeshComponent.h"
#include "..\interfaces\RenderableInterface.h"

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

	std::shared_ptr<ShaderProgram> getShaderProgram(std::string name);
	int const getWindowWidth() const;
	int const getWindowHeight() const;

	void addRenderable(std::string name, std::shared_ptr<RenderableInterface> objectPtr);
	void removeRenderable(std::string name);

protected:
	virtual void onAbort();

private:
	ShaderFactory shaderFactory;

	std::shared_ptr<Camera> camera;

	bool usePerspectiveMode;

	int windowWidth;
	int windowHeight;

	//bool initShaders();
	void setProjectionMode(ProjectionMode mode = ProjectionMode::PERSPECTIVE);

	// Map with shaderPrograms;
	std::map<std::string, std::shared_ptr<ShaderProgram>> shaderPrograms;

	// Map with MeshComponents
	std::map<std::string, std::shared_ptr<RenderableInterface>> renderables;

	/* test with mesh */
	std::shared_ptr<ContainerOBJ> car;

	// A singleton instance
	static std::shared_ptr<Renderer> instance;
};

