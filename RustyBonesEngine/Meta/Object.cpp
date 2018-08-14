#include "../Core.Engine.h"
#include "Object.h"

namespace rusty
{
	MetaClass * const Object::get_meta() const
	{
		return m_meta;
	}
}
