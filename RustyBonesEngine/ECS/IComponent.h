#pragma once

#include "../Meta/Object.h"

namespace rusty
{
	class IComponent : public Object
	{
	public:
		void operator delete(void* ptr);
		void operator delete[](void* ptr);
	};
}