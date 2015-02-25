#include "ProcessManager.h"

ProcessManager::~ProcessManager(void)
{
	this->clearAllProcesses();
}

unsigned int ProcessManager::updateProcesses(unsigned long deltaMs)
{
	//printf("Entering ProcessManager::updateProcesses(%lu).\n", deltaMs);

	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	std::list<std::shared_ptr<Process>>::iterator it = this->processList.begin();

	// For all processes in list
	while (it != this->processList.end())
	{
		std::shared_ptr<Process> currentProcess = (*it);
		std::list<std::shared_ptr<Process>>::iterator thisIt = it;

		++it;

		// Initialize when necessary
		if (currentProcess->getState() == Process::UNINITIALIZED)
		{
			currentProcess->init();
		}

		// Update thos that are running
		if (currentProcess->getState() == Process::RUNNING)
		{
			currentProcess->update(deltaMs);
		}

		// Decide what to do with dead processes before removing them
		if (currentProcess->isDead())
		{
			switch (currentProcess->getState())
			{
				case Process::SUCCEEDED:
				{
					currentProcess->onSuccess();

					// handle possible child
					std::shared_ptr<Process> child = currentProcess->removeChild();

					if (child)
					{
						attachProcess(child);
					}
					else
					{
						++successCount;
					}
					break;
				}
				case Process::FAILED:
				{
					currentProcess->onFail();
					++failCount;
					break;
				}
				case Process::ABORTED:
				{
					currentProcess->onAbort();
					++failCount;
					break;
				}
			} // end switch

			// remove process from processlist
			processList.erase(thisIt);

		} // end if dead
	} // end while(processList)
	
	return ((successCount << 16) | failCount);

} // end updateProcesses(

std::weak_ptr<Process> ProcessManager::attachProcess(std::shared_ptr<Process> process)
{
	// TODO: Do we need to check if it exists?
	this->processList.push_front(process);

	return std::weak_ptr<Process>(process);
}

void ProcessManager::abortAllProcesses(bool immediately)
{
	std::list<std::shared_ptr<Process>>::iterator it = processList.begin();
	std::list<std::shared_ptr<Process>>::iterator currentIt;
	std::shared_ptr<Process> currentProcess;

	while (it != processList.end())
	{
		currentIt = it;
		++it;
		currentProcess = (*currentIt);

		if (currentProcess->isAlive())
		{
			currentProcess->setState(Process::ABORTED);
			if (immediately)
			{
				currentProcess->onAbort();
				processList.erase(currentIt);
			}
		}

	}
}

void ProcessManager::clearAllProcesses(void)
{
	this->processList.clear();
}

