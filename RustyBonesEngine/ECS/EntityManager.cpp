#include "../Core.Engine.h"
#include "EntityManager.h"

#include "ComponentSystem.h"
#include "IComponent.h"

namespace rusty
{
	Entity EntityManager::create_entity()
	{
		if (!m_free_list.empty())
		{
			auto entity = m_free_list[0];
			entity.m_version++;
			m_free_list[0] = m_free_list[m_free_list.size() - 1];
			m_free_list.pop_back();
			m_entities.emplace(std::make_pair(entity, EntityVector()));
			return entity;
		}

		m_last_entity = m_last_entity + 1;

		Entity entity(m_last_entity);
		m_entities.emplace(std::make_pair(entity, EntityVector()));

		return entity;
	}

	// more work to be done on create/destroy
	void EntityManager::destroy_entity(const Entity &entity)
	{
		m_free_list.emplace_back(entity);

		// TODO: add in code to release components now that entity is destroyed
	}

	void EntityManager::update(float dt)
	{
		for (std::size_t i = 0; i < m_systems.size(); i++)
		{
			m_systems[i]->update_system(dt);
		}
	}
}