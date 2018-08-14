#include "../Core.Engine.h"
#include "IComponent.h"

void rusty::IComponent::operator delete(void * ptr)
{
	Fail();
}

void rusty::IComponent::operator delete[](void * ptr)
{
	Fail();
}
