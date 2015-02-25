#pragma once

#include <memory>
#include <list>
#include "Process.h"

class ProcessManager
{
	std::list<std::shared_ptr<Process>> processList;

public:
	~ProcessManager(void);

	unsigned int updateProcesses(unsigned long deltaMs);
	std::weak_ptr<Process> attachProcess(std::shared_ptr<Process> process);
	void abortAllProcesses(bool immediately);

	unsigned int getProcessCount(void) const{ return this->processList.size(); }

private: 
	void clearAllProcesses(void);
};