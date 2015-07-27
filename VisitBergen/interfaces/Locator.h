#pragma once

#include <memory>

#include "..\renderer\Renderer.h"
#include "..\actorManager\ActorManager.h"

class Renderer;

class Locator
{
	static std::shared_ptr<Renderer> renderer;
	static std::shared_ptr<ActorManager> actorManager;
public:
	static void provideRenderer(std::shared_ptr<Renderer> service);
	static void provideActorManager(std::shared_ptr<ActorManager> service);
	static std::shared_ptr<Renderer> getRenderer();
	static std::shared_ptr<ActorManager> getActorManager();
};
