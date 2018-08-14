#pragma once

#include "../ECS/IComponent.h"

/**
* @reflect Serialize;
*/
class MovementComponent : public rusty::IComponent
{
public:
	float x;
	float y;
	float z;
	float speed;
};