#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::init(void)
{
	std::printf("Hello from Renderer!");
}

void Renderer::update(unsigned long deltaMs)
{
	return;
}

void Renderer::handleEvent(const std::shared_ptr<Event>& event)
{
	return;
}
