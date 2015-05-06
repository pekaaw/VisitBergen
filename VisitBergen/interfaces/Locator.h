#pragma once

#include <memory>

#include "..\renderer\Renderer.h"

class Renderer;

class Locator
{
	static std::shared_ptr<Renderer> renderer;
public:
	static void provideRenderer(std::shared_ptr<Renderer> service);
	static std::shared_ptr<Renderer> getRenderer();
};
