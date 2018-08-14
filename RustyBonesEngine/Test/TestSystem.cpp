#include "../Core.Engine.h"
#include "TestSystem.h"

#include "MovementComponent.hpp"

#include <iostream>

void TestSystem::update(float dt, MovementComponent* movement)
{
	std::cout << "Test system update called" << std::endl;
	movement->x += 10;
	std::cout << "X: " << movement->x << " Y: " << movement->y << " Z: " << movement->z << std::endl;
}
