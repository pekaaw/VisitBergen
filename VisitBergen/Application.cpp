#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32")

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "gl\GL.h"

#include <iostream>

#include "Application.h"
#include "processes\Timer.h"
#include "processes\TestProcess.h"


Application::Application()
{
}


Application::~Application()
{
}

int Application::init(int* argc, char** argv)
{
	/* TODO:
		- Detect multiple instances of the application
		- Check secondary storage space and memory
		- Calculate the CPU speed
		- Load resource cache
		- Load string messages for application
		- Create LUA Script Manager
		- Create Event Manager
		- Script Manager to load initial options
		- Initialize window and input devices
		- Create game logic and game views
		- Set directory for saving sessions and temporary files
		- Preload selected resources from the resource cache
	*/

	this->eventManager = std::make_shared<EventManager>();
	this->processManager = std::make_shared<ProcessManager>();

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(this->windowWidht, this->windowHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(this->windowTitle);

	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "Error initializing glew: \n"
			<< reinterpret_cast<const char*>(glewGetErrorString(result)) << std::endl;
		return -1;
	}

	this->renderer = std::make_shared<Renderer>();
	this->renderer->init();

	//glutDisplayFunc(nullptr);
	//glutIdleFunc(nullptr);
	//glutKeyboardFunc(nullptr);
	//glutMouseFunc(nullptr);
	//glutMotionFunc(nullptr);

	return 1;
}

int Application::execute()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::high_resolution_clock> currTime;

	std::shared_ptr<Process> timer = std::make_shared<Timer>(2000);
	std::shared_ptr<Process> testProcess = std::make_shared<TestProcess>();
	timer->attachChild(testProcess);

	this->processManager->attachProcess(timer);

	unsigned long duration;
	unsigned long prefDuration = unsigned long (1000 / this->preferredFPS);

	std::printf("Preferred Duration: %lu\n", prefDuration);

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

		this->processManager->updateProcesses(duration);
		
	}

	return 0;
}

int Application::shutdown()
{
	/*
		- Release resources
		- Save session to save directory and remove temporary files
		- Shutdown game logic and game views
		- Shutdown/release window and input devices
		- Shutdown Event Manager
		- Shutdown Script Manager
		- Delete string messages
		- Shutdown resource cache
		- ... make sure a graceful exit will happen
	*/
	return 0;
}