#include "TransformComponent.h"

const char* TransformComponent::componentName = "TransformComponent";

bool TransformComponent::preInit(tinyxml2::XMLElement* data)
{
	glm::mat4 translation;
	glm::mat4 yawPitchRoll;

	tinyxml2::XMLElement* position = data->FirstChildElement("Position");
	if (position)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		position->QueryFloatAttribute("x", &x);
		position->QueryFloatAttribute("y", &y);
		position->QueryFloatAttribute("z", &z);

		translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		printf("creating translation matrix!\n");
	}

	tinyxml2::XMLElement* orientation = data->FirstChildElement("YawPitchRoll");
	if (orientation)
	{
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		orientation->QueryFloatAttribute("yaw", &yaw);
		orientation->QueryFloatAttribute("pitch", &pitch);
		orientation->QueryFloatAttribute("roll", &roll);

		yawPitchRoll = glm::yawPitchRoll(yaw, pitch, roll);
		printf("creating yawPitchRoll matrix!\n");
	}

	this->worldTransform = yawPitchRoll * translation;

	return true;
}