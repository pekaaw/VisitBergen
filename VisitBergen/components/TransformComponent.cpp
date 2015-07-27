#include "TransformComponent.h"

const char* TransformComponent::componentName = "TransformComponent";

bool TransformComponent::preInit(tinyxml2::XMLElement* data)
{
	glm::mat4 translation;
	glm::mat4 yawPitchRoll;
	glm::mat4 scaling;

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

	tinyxml2::XMLElement* scale = data->FirstChildElement("Scaling");
	if (scale)
	{
		glm::vec3 scaleVector;

		// Default scale to 1. If query successful, the scale is updated.
		float total = 1;
		tinyxml2::XMLError msg = scale->QueryFloatText(&total);
		scaleVector = glm::vec3(total, total, total);

		// If the attributes are set, the corresponding dimension are updated.
		scale->QueryFloatAttribute("x", &scaleVector.x);
		scale->QueryFloatAttribute("y", &scaleVector.y);
		scale->QueryFloatAttribute("z", &scaleVector.z);

		// create the scaling matrix!
		scaling = glm::scale(glm::mat4(1.0f), scaleVector);
		printf("creating scaling matrix!\n");
	}

	// set the final transform matrix
//	this->transform = scaling * yawPitchRoll * translation;
	this->transform = translation * yawPitchRoll * scaling;

	return true;
}

// read-only
const glm::mat4& TransformComponent::getTransform() const
{
	return this->transform;
}

void TransformComponent::setTransform(glm::mat4& matrix)
{
	this->transform = matrix;
}