#include "Application.h"

int main(int argc, char** argv)
{
	Application application;
	int exitCode = -1;

	if (application.init(&argc, argv))
	{
		exitCode = application.execute();
	}

	if (exitCode == 0)
	{
		exitCode = application.shutdown();
	}
	
	std::getchar();

	return exitCode;
}