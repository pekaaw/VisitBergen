#pragma once

#include <memory>
#include <string>
#include "../shader/ShaderProgram.h"
#include "RenderableInterface.h"

class RendererInterface
{
public:
	virtual std::shared_ptr<ShaderProgram> getShaderProgram(std::string name) = 0;
	virtual void addRenderable(std::string name, std::shared_ptr<RenderableInterface> objectPtr) = 0;
	virtual void removeRenderable(std::string name) = 0;
};