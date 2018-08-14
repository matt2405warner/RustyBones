#pragma once

#include "../ECS/IComponent.h"

#include <string>

/**
* @reflect Serialize;
*/
class GraphicsComponent : public rusty::IComponent
{
public:
	std::string shader_name;
};