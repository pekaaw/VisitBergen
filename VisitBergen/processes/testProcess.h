#pragma once
#include <stdio.h>
#include "../processManager/Process.h"

class TestProcess : public Process
{
public:
	TestProcess(void){};
	~TestProcess(void){};

	virtual void update(unsigned long deltaMs)
	{
		printf("Hello from TestProcess!\n");
		this->succeed();
	}
};