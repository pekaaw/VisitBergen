

#include "ArcBall.h"

ArcBall::ArcBall(int width, int height)
{
	this->startVector = glm::vec3(0.0f);
	this->endVector = glm::vec3(0.0f);
	
	this->setDimensions(width, height);
}

void ArcBall::setDimensions(int width, int heigth)
{
	this->widthAdjustmentFactor = 1.0f / (width - 1.0f) * 0.5f;
	this->heightAdjustmentFactor = 1.0f / (heigth - 1.0f) * 0.5f;
}

void ArcBall::mapToSphere(int x, int y, glm::vec3* vector)
{
	float x_ = (x * this->widthAdjustmentFactor) - 1;
	float y_ = 1 - (y * this->heightAdjustmentFactor);

	float length = x_*x_ + y_*y_;

	// Outside of ArcBall
	if (length > 1.0f)
	{
		float normalizationFactor = 1.0f / sqrt(length);

		vector->x = x_ * normalizationFactor;
		vector->y = y_ * normalizationFactor;
		vector->z = 0.0f;
	}
	else // inside ArcBall
	{
		vector->x = x_;
		vector->y = y_;
		vector->z = sqrt(1.0f - length);
	}
}

	//glm::vec3 startVector;
	//glm::vec3 endVector;

	//float widthAdjustmentFactor;
	//float heightAdjustmentFactor;

void ArcBall::handleEvent(const std::shared_ptr<Event>& event_)
{

}