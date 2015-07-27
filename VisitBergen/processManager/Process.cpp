#include "Process.h"

Process::~Process(void)
{
	if (this->childProcess)
	{
		this->childProcess->onAbort();
	}
}

Process::Process(void)
{
	this->state = Process::UNINITIALIZED;
}


std::shared_ptr<Process> Process::removeChild(void)
{
	if (this->childProcess)
	{
		std::shared_ptr<Process> child = this->childProcess;
		this->childProcess.reset();
		return child;
	}

	return std::shared_ptr<Process>();
}

void Process::succeed(void)
{
	this->state = Process::SUCCEEDED;
}

inline void Process::fail(void)
{
	this->state = Process::FAILED;
}

void Process::attachChild(std::shared_ptr<Process> child)
{
	if (this->childProcess)
	{
		this->childProcess->attachChild(child);
	}
	else
	{
		this->childProcess = child;
	}
}

void Process::pause(void)
{
	if (this->state == Process::RUNNING)
	{
		this->state = Process::PAUSED;
	}
}

void Process::unPause(void)
{
	if (this->state == Process::PAUSED)
	{
		this->state = Process::RUNNING;
	}
}