#pragma once

#include <memory>

#include "EntityManager.h"

namespace rusty
{
	class World : public Singleton<World>
	{
	private:
		std::unique_ptr<EntityManager> m_em_instance;

		template <typename T>
		T* get_or_create_instance_impl()
		{
			return nullptr;
		}

		template <>
		EntityManager* get_or_create_instance_impl<EntityManager>()
		{
			if (m_em_instance == nullptr)
			{
				m_em_instance = std::make_unique<EntityManager>();
			}
			return m_em_instance.get();
		}

	public:
		template <class T>
		T* get_or_create_instance()
		{
			return get_or_create_instance_impl<T>();
		}
	};
}