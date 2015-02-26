#pragma once

#include <chrono>
#include <cstdio>
#include <thread>
#include "eventManager\EventManager.h"
#include "processManager\ProcessManager.h"
#include "renderer\Renderer.h"

class Application
{
private:
	std::shared_ptr<EventManager> eventManager;
	std::shared_ptr<ProcessManager> processManager;
	std::shared_ptr<Renderer> renderer;
	//EventManager* eventManager;
	//ProcessManager* processManager;
	//Renderer* renderer;
	
	bool quit = 0;
	int preferredFPS = 60;

	int windowWidht = 800;
	int windowHeight = 600;
	char* windowTitle = "Visit Bergen\0";

public:
	Application();
	~Application();

	int init(int* argc, char** argv);
	int execute();
	int shutdown();
};

