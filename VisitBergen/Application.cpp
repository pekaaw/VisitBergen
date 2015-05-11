#include "util\includeGL.h"

#include <iostream>

#include "Application.h"
#include "interfaces\Locator.h"
#include "processes\Timer.h"
#include "processes\TestProcess.h"
#include "processes\CameraFlightProcess.h"
#include "events\QuitApplication.h"
#include "events\EventCameraTransform.h"
#include "events\EventCameraTrip.h"
#include "events\EventToggleProjectionMode.h"
#include "events\EventToggleModelRotation.h"

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

	this->processManager = std::make_shared<ProcessManager>();
	this->inputHandler = InputHandler::getInstance();
	this->eventManager = EventManager::getInstance();
	this->actorManager = std::make_shared<ActorManager>();
	this->renderer = Renderer::getInstance();

	Locator::provideRenderer(this->renderer);

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(this->renderer->getWindowWidth(), this->renderer->getWindowHeight());
	glutInitWindowPosition(470, 0);
	glutCreateWindow(this->windowTitle);

	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "Error initializing glew: \n"
			<< reinterpret_cast<const char*>(glewGetErrorString(result)) << std::endl;
		return -1;
	}


	glutKeyboardFunc(this->inputHandler->keyboardCall);
	glutMouseFunc(this->inputHandler->mouseCall);
	glutMouseWheelFunc(this->inputHandler->mouseWheelCall);
	glutMotionFunc(this->inputHandler->motionCall);
	glutDisplayFunc(this->empty);
	glutReshapeFunc(this->reshape);

	this->renderer->getShaderProgram("phongLighting");

	return 1;
}

void Application::reshape(int width, int height)
{
	Locator::getRenderer()->reshape(width, height);
}

void Application::empty(){}

int Application::execute()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> prevTime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::high_resolution_clock> currTime;

	this->actorManager->createActor("actors//car.xml");

	std::shared_ptr<Process> timer = std::make_shared<Timer>(2000);
	std::shared_ptr<Process> testProcess = std::make_shared<TestProcess>();
	timer->attachChild(testProcess);

	this->processManager->attachProcess(timer);
	this->processManager->attachProcess(this->renderer);

	this->eventManager->addListener(shared_from_this(), std::make_shared<QuitApplication>());
	this->eventManager->addListener(shared_from_this(), std::make_shared<EventCameraTrip>());
	this->eventManager->addListener(this->renderer, std::make_shared<EventCameraTransform>());
	this->eventManager->addListener(this->renderer, std::make_shared<EventToggleProjectionMode>());
	this->eventManager->addListener(this->renderer, std::make_shared<EventToggleModelRotation>());

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

		glutMainLoopEvent();

		this->update();
		this->eventManager->update();
		this->processManager->updateProcesses(duration);

	}

	printf("Exit Application::execute()\n");
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

	printf("Shutdown renderer...\n");
	this->renderer.reset();

	printf("Shutdown ActorManager...\n");
	this->actorManager.reset();

	printf("Shutdown EventManager...\n");
	this->eventManager.reset();

	printf("Shutdown InputHandler...\n");
	this->inputHandler.reset();

	printf("Shutdown ProcessManager...\n");
	this->processManager.reset();

	return 0;
}

void Application::handleEvent(const std::shared_ptr<Event>& event_)
{
	if (std::shared_ptr<QuitApplication> quitMe = std::dynamic_pointer_cast<QuitApplication>(event_))
	{
		quitMe->sayHallo();
		this->quit = true;
	}

	if (std::shared_ptr<EventCameraTrip> trip = std::dynamic_pointer_cast<EventCameraTrip>(event_))
	{
		this->processManager->attachProcess(std::make_shared<CameraFlightProcess>(3000));
	}
}