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
	EventManager eventManager;
	ProcessManager processManager;
	Renderer renderer;
	
	bool quit = 0;
	//int preferredFPS = 60;
	int preferredFPS = 1;

public:
	Application();
	~Application();

	int init();
	int execute();
	int shutdown();
};

