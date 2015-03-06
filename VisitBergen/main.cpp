#include "Application.h"

int main(int argc, char** argv)
{
	std::shared_ptr<Application> application = std::make_shared<Application>();
//	Application application;
	int exitCode = -1;

	if (application->init(&argc, argv))
	{
		exitCode = application->execute();
	}

	if (exitCode == 0)
	{
		exitCode = application->shutdown();
	}

	application.reset();

	printf("Leaving main. Goodbye.");
	return exitCode;
}