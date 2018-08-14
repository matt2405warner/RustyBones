#pragma once

#include <cstdlib>

namespace rusty
{
	using EntityValue = std::size_t;

	class Entity
	{
		friend class EntityManager;

	private:
		EntityValue m_id;
		EntityValue m_version;

	public:
		Entity();
		explicit Entity(EntityValue id);
		EntityValue id() const;
		EntityValue version() const;

		bool operator<(const Entity& rhs) const;
		bool operator==(const Entity& rhs) const;
		bool operator!=(const Entity& rhs) const;
		bool operator>(const Entity& rhs) const;
		bool operator<=(const Entity& rhs) const;
		bool operator>=(const Entity& rhs) const;
		friend std::ostream& operator<<(std::ostream& stream, const Entity& entity);
	};
}