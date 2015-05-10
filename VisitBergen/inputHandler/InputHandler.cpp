#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "InputHandler.h"
#include "..\eventManager\EventManager.h"
#include "..\events\QuitApplication.h"
#include "..\events\EventCameraTransform.h"
#include "..\events\EventCameraTrip.h"
#include "..\events\EventToggleProjectionMode.h"
#include "..\events\EventToggleModelRotation.h"

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

void InputHandler::mouseWheelCall(int button, int direction, int x, int y)
{
	InputHandler::getInstance()->mouseWheel(button, direction, x, y);
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
		EventManager::getInstance()->fireEvent(std::make_shared<QuitApplication>());
		printf("Quit!\n");
		break;

	case 'p':
		EventManager::getInstance()->fireEvent(std::make_shared<EventToggleProjectionMode>());
		printf("Toggle projection mode.\n");
		break;

	case 'r':
		EventManager::getInstance()->fireEvent(std::make_shared<EventToggleModelRotation>());
		printf("Toggle model rotation.\n");
		break;

	case 't':
		EventManager::getInstance()->fireEvent(std::make_shared<EventCameraTrip>());
		printf("Take a trip!\n");
		break;
	}
}

void InputHandler::mouse(int button, int state, int x, int y)
{
	this->mouseButton = button;
	this->mouseX = x;
	this->mouseY = y;
}

void InputHandler::mouseWheel(int button, int direction, int x, int y)
{
	if (direction > 0)
	{
		// zoom in
		glm::mat4 zoom = glm::scale(glm::mat4(), glm::vec3(1.05f));
		std::shared_ptr<EventCameraTransform> cameraTransform = std::make_shared<EventCameraTransform>();
		cameraTransform->setTransform(zoom);
		EventManager::getInstance()->fireEvent(cameraTransform);
	}
	else
	{
		// zoom out
		glm::mat4 zoom = glm::scale(glm::mat4(), glm::vec3(0.95f));
		std::shared_ptr<EventCameraTransform> cameraTransform = std::make_shared<EventCameraTransform>();
		cameraTransform->setTransform(zoom);
		EventManager::getInstance()->fireEvent(cameraTransform);
	}
}

void InputHandler::motion(int x, int y)
{
	if (this->mouseButton == GLUT_LEFT_BUTTON)
	{
		glm::mat4 rx, ry;
		rx = glm::rotate(rx, -0.001f * (this->mouseY - y), glm::vec3(1.0f, 0.0f, 0.0f));
		ry = glm::rotate(ry, -0.001f * (this->mouseX - x), glm::vec3(0.0f, 1.0f, 0.0f));

		std::shared_ptr<EventCameraTransform> cameraTransform = std::make_shared<EventCameraTransform>();
		cameraTransform->setTransform(rx * ry);

		EventManager::getInstance()->fireEvent(cameraTransform);
	}

	if (this->mouseButton == GLUT_RIGHT_BUTTON)
	{
		glm::mat4 translate;
		glm::vec3 direction;
		direction.x = -0.001 * (this->mouseX - x);
		direction.y = 0.001 * (this->mouseY - y);
		translate = glm::translate(translate, direction);
		std::shared_ptr<EventCameraTransform> cameraTransform = std::make_shared<EventCameraTransform>();
		cameraTransform->setTransform(translate);

		EventManager::getInstance()->fireEvent(cameraTransform);
	}

	this->mouseX = x;
	this->mouseY = y;
}