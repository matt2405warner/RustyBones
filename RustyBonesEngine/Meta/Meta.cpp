#include "../Core.Engine.h"
#include "Meta.h"

#include <algorithm>

namespace rusty
{

	MetaClass * MetaDB::get_meta(const std::string & name)
	{
		auto found_meta = std::find_if(m_types.begin(), m_types.end(), [name](MetaClass* meta)
		{
			if (meta == nullptr)
				return false;

			return meta->get_name() == name;
		});
		return *found_meta;
	}

	MetaClass * MetaDB::get_meta(std::size_t id)
	{
		if (id < 0 || id >= m_types.size())
			return nullptr;

		return m_types[id];
	}

	void MetaDB::add_type(MetaClass * meta)
	{
		if (meta == nullptr || meta->get_id() < 0)
			return;

		auto meta_id = meta->get_id();
		if (m_types.empty() || meta_id >= m_types.size())
		{
			m_types.resize(meta_id + 1);
		}

		m_types[meta_id] = meta;
	}
}
