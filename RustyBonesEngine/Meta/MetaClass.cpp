#include "../Core.Engine.h"
#include "MetaClass.h"

namespace rusty
{
	MetaMember::MetaMember(const std::string & name, std::size_t offset)
		: m_name(name), m_offset(offset)
	{
	}

	MetaFunction::MetaFunction(const std::string & name, std::size_t offset)
		: m_name(name), m_offset(offset)
	{
	}

	MetaClass::MetaClass(MetaId id, const std::string & name, std::size_t size, ObjectCtor ctor, ObjectCtorInPlace inplace, ObjectDtor dtor)
		: m_id(id), m_name(name), m_size(size), m_ctor(ctor), m_ctor_inplace(inplace), m_dtor(dtor)
	{
	}

	void MetaClass::destroy(Object * obj) const
	{
		m_dtor(obj);
	}

	const std::string & MetaClass::get_name() const
	{
		return m_name;
	}

	const std::size_t MetaClass::get_id() const
	{
		return m_id;
	}

	const std::size_t MetaClass::get_size() const
	{
		return m_size;
	}
}
