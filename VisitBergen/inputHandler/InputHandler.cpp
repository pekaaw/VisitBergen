#include "InputHandler.h"
#include "..\eventManager\EventManager.h"
#include "..\events\QuitApplication.h"

std::shared_ptr<InputHandler> InputHandler::instance = nullptr;

const std::shared_ptr<InputHandler> InputHandler::getInstance(void)
{
	if (InputHandler::instance == nullptr)
	{
		InputHandler::instance = std::make_shared<InputHandler>();
	}

	return InputHandler::instance;
}


void InputHandler::keyboardCall(unsigned char key, int x, int y)
{
	InputHandler::getInstance()->keyboard(key, x, y);
}

void InputHandler::mouseCall(int button, int state, int x, int y)
{
	InputHandler::getInstance()->mouse(button, state, x, y);
}

void InputHandler::motionCall(int x, int y)
{
	InputHandler::getInstance()->motion(x, y);
}

void InputHandler::keyboard(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'q':
		//std::shared_ptr<Event> quit = std::make_shared<QuitApplication>();
		//EventManager::getInstance()->fireEvent(quit);
		EventManager::getInstance()->fireEvent(std::make_shared<QuitApplication>());
		// create exit event;
		printf("Quit!\n");
		break;
	}
}

void InputHandler::mouse(int button, int state, int x, int y)
{

}

void InputHandler::motion(int x, int y)
{

}