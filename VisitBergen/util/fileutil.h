#pragma once
#include <string>
#include <fstream>

inline std::string readTextFile(const std::string& pathAndFileName)
{
	std::ifstream fileIn(pathAndFileName);
	if (!fileIn.is_open())
	{
		printf("Error: Unable to open file $s\n", pathAndFileName.c_str());
		return "";
	}

	// Read the file
	std::string text = "";
	std::string line;
	while (!fileIn.eof())
	{
		std::getline(fileIn, line);
		text += line + "\n";

		if (fileIn.bad() || (fileIn.fail() && !fileIn.eof()))
		{
			printf("Warning: Problems while reading file %s\n Text read:\n %s \n", pathAndFileName.c_str(), text.c_str());
			fileIn.close();
			return text;
		}
	}

	fileIn.close();

	return text;
}