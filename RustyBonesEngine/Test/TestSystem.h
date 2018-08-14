#pragma once

#include "../ECS/ComponentSystem.h"

class MovementComponent;

class TestSystem : public rusty::ComponentSystem<TestSystem, MovementComponent>
{
public:
	void update(float dt, MovementComponent* movement);
};