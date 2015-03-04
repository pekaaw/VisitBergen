#pragma once

#include <chrono>
#include <cstdio>
#include <thread>
#include "eventManager\EventManager.h"
#include "eventManager\EventListener.h"
#include "processManager\ProcessManager.h"
#include "renderer\Renderer.h"
#include "inputHandler\InputHandler.h"

#include "events\QuitApplication.h"

class Application :
	public EventListener,
	public std::enable_shared_from_this<Application>
{
private:
	std::shared_ptr<EventManager> eventManager;
	std::shared_ptr<ProcessManager> processManager;
	std::shared_ptr<Renderer> renderer;
	//EventManager* eventManager;
	//ProcessManager* processManager;
	//Renderer* renderer;
	std::shared_ptr<InputHandler> inputHandler;
	
	bool quit = 0;
	int preferredFPS = 60;

	int windowWidht = 800;
	int windowHeight = 600;
	char* windowTitle = "Visit Bergen\0";

	void handleEvent(const std::shared_ptr<Event>& event);

public:
	Application();
	~Application();

	int init(int* argc, char** argv);
	int execute();
	int shutdown();
};

