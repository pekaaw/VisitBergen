#include "Locator.h"


// this must be in a .cpp file =(
std::shared_ptr<Renderer> Locator::renderer = nullptr;



void Locator::provideRenderer(std::shared_ptr<Renderer> service)
{
	renderer = service;
}



std::shared_ptr<Renderer> Locator::getRenderer()
{
	return renderer;
}