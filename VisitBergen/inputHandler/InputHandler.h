#pragma once

#include <memory>

#include "../util/includeGL.h"

class InputHandler
{
	static std::shared_ptr<InputHandler> instance;

	void keyboard(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void mouseWheel(int button, int direction, int x, int y);
	void motion(int x, int y);

public:
	static const std::shared_ptr<InputHandler> getInstance(void);

	static void keyboardCall(unsigned char key, int x, int y);
	static void mouseCall(int button, int state, int x, int y);
	static void mouseWheelCall(int button, int direction, int x, int y);
	static void motionCall(int x, int y);

private:
	int mouseButton;
	int mouseX;
	int mouseY;
};