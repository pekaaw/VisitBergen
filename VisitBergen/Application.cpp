#include "Application.h"
#include "processes\Timer.h"
#include "processes\TestProcess.h"


Application::Application()
{
}


Application::~Application()
{
}

int Application::init()
{
	return 1;
}

int Application::execute()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::high_resolution_clock> currTime;

	std::shared_ptr<Process> timer = std::make_shared<Timer>(2000);
	std::shared_ptr<Process> testProcess = std::make_shared<TestProcess>();
	timer->attachChild(testProcess);

	this->processManager.attachProcess(timer);

	long long duration;
	long long prefDuration = 1000.0 / unsigned long(this->preferredFPS);
	long long accumulator = 0;

	while (!this->quit)
	{
		currTime = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count();
		prevTime = currTime;

		if (duration < prefDuration)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((prefDuration - duration)/2));
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime).count();
		}

		this->processManager.updateProcesses(duration);
		
	}

	return 0;
}

int Application::shutdown()
{
	return 0;
}