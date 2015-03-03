#include "InputHandler.h"

std::shared_ptr<InputHandler> InputHandler::instance = nullptr;

std::shared_ptr<InputHandler> InputHandler::getInstance(void)
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
		// create exit event;
		break;
	}
}

void InputHandler::mouse(int button, int state, int x, int y)
{

}

void InputHandler::motion(int x, int y)
{

}