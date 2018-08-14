#include "../Core.Engine.h"
#include "Entity.h"

namespace rusty
{
	EntityValue Entity::id() const
	{
		return m_id;
	}

	EntityValue Entity::version() const
	{
		return m_version;
	}

	bool Entity::operator<(const Entity &rhs) const
	{
		return (m_id < rhs.m_id) || (m_id == rhs.m_id && m_version < rhs.m_version);
	}

	bool Entity::operator==(const Entity &rhs) const
	{
		return m_id == rhs.m_id && m_version == rhs.m_version;
	}

	bool Entity::operator>(const Entity &rhs) const
	{
		return m_id > rhs.m_id || (m_id == rhs.m_id && m_version > rhs.m_version);
	}

	bool Entity::operator!=(const Entity &rhs) const
	{
		return !(*this == rhs);
	}

	bool Entity::operator<=(const Entity &rhs) const
	{
		return (*this < rhs) || (*this == rhs);
	}

	bool Entity::operator>=(const Entity &rhs) const
	{
		return (*this > rhs) || (*this == rhs);
	}

	Entity::Entity()
		: m_id(1), m_version(1)
	{

	}

	Entity::Entity(EntityValue id)
		: m_id(id), m_version(1)
	{

	}
	std::ostream & operator<<(std::ostream & stream, const Entity & entity)
	{
		stream << "Entity index: " << entity.id() << ", version: " << entity.version();
		return stream;
	}
}