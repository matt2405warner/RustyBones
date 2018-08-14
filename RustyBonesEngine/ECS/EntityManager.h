#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>

#include "Entity.h"
#include "Pool.h"

namespace rusty
{
	namespace impl
	{
		template <class T>
		void get_type_impl(std::vector<MetaClass*>& types)
		{
			types.emplace_back(rusty::get_type<T>());
		}
		template<class T0, class T1, class... Tn>
		void get_type_impl(std::vector<MetaClass*>& types)
		{
			types.emplace_back(rusty::get_type<T0>());
			impl::get_type_impl<T1, Tn...>(types);
		}
	}

	template <class... T>
	std::vector<MetaClass*> get_list_types()
	{
		std::vector<MetaClass*> types;
		types.reserve(sizeof...(T));
		impl::get_type_impl<T...>(types);
		return types;
	}

	template <class... T>
	std::tuple<T...> get_types()
	{
		return std::make_tuple(rusty::get_type<T>()...);
	}

	class IComponent;
	class IComponentSystem;

	using ComponentId = std::size_t;

	class EntityManager
	{
	public:
		using ComponentPair = std::pair<ComponentId, std::size_t>;
		using EntityVector = std::vector<ComponentPair>;

	private:
		std::map<Entity, EntityVector> m_entities;
		std::map<ComponentId, std::unique_ptr<BasePool>> m_components;
		std::vector<std::unique_ptr<IComponentSystem>> m_systems;

		std::vector<Entity> m_free_list;
		EntityValue m_last_entity;

	public:
		Entity create_entity();
		void destroy_entity(const Entity& entity);
		void update(float dt);
		template <class T>
		T* add_component(const Entity& entity)
		{
			auto meta = rusty::get_type<T>();
			if (meta == nullptr)
				return nullptr;

			auto entity_vec = m_entities.find(entity);

			// valid entity was not found to be active
			if (entity_vec == m_entities.end())
				return nullptr;

			auto comp_vec = m_components.find(meta->get_id());

			std::size_t  index = 0;
			T* component = nullptr;
			// if component type not found then create vector and place first component in map
			if (comp_vec == m_components.end())
			{
				auto pool = std::make_unique<SlotPool<T>>(1);

				auto comp_pair = pool->create();
				component = comp_pair.first;
				index = comp_pair.second;
				m_components[meta->get_id()] = std::move(pool);
			}
			// resize linear allocator and in-place allocate new component
			else {
				auto pool = reinterpret_cast<SlotPool<T>*>(comp_vec->second.get());
				auto comp_pair = pool->create();
				component = comp_pair.first;
				index = comp_pair.second;
			}

			// tie new component and entity together
			entity_vec->second.emplace_back(meta->get_id(), index);

			//return ComponentWrapper<T>(component);
			return component;
		}

		template <class T>
		bool remove_component(const Entity& entity)
		{
			// find the entity
			auto entity_vec = m_entities.find(entity);

			if (entity_vec == m_entities.end())
				return false;

			// get the meta type
			auto meta = rusty::get_type<T>();
			if (meta == nullptr)
				return false;

			auto comp_type_id = meta->get_id();
			// find the pair for the component vector
			auto ent_comp = std::find_if(entity_vec->second.begin(), entity_vec->second.end(),
				[&comp_type_id](const std::pair<ComponentId, std::size_t>& elem) {
				return comp_type_id == elem.first;
			});
			if (ent_comp == entity_vec->second.end())
				return false;

			// find component vector in component map
			auto comp_vec = m_components.find(ent_comp->first);
			if (comp_vec == m_components.end())
				return false;

			// perform removal from entity component list and component list
			entity_vec->second.erase(ent_comp);
			comp_vec->second->destroy(ent_comp->second);

			return true;
		}

		template <class T>
		T* get_component(const Entity& entity)
		{
			auto e_it = m_entities.find(entity);

			// unable to find active entity
			if (e_it == m_entities.end())
				return nullptr;

			MetaClass* meta = rusty::get_type<T>();
			auto meta_id = meta->get_id();
			auto c_pair = std::find_if(e_it->second.begin(), e_it->second.end(), [meta_id](const ComponentPair& comp_pair) {
				return meta_id == comp_pair.first;
			});
			// unable to find component registered with entity
			if (c_pair == e_it->second.end())
				return nullptr;

			auto comp_it = m_components.find(c_pair->first);
			if (comp_it == m_components.end())
				return nullptr;

			auto comp = comp_it->second->get(c_pair->second);
			if (comp == nullptr)
				return nullptr;

			//ComponentWrapper<T> c(static_cast<T*>(comp));
			return static_cast<T*>(comp);
		}

		template <class T>
		bool has_component(const Entity& entity) const
		{
			static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent to be registered with Entity.");
			auto e_it = m_entities.find(entity);

			// unable to find active entity
			if (e_it == m_entities.end())
				return false;

			MetaClass* meta = rusty::get_type<T>();
			// unable to find meta for type
			if (meta == nullptr)
				return false;

			auto meta_id = meta->get_id();
			auto c_pair = std::find_if(e_it->second.begin(), e_it->second.end(), [meta_id](const ComponentPair& comp_pair) {
				return meta_id == comp_pair.first;
			});

			return c_pair != e_it->second.end();
		}

		bool has_component(const Entity& entity, MetaClass* meta) const
		{
			if (meta == nullptr)
				return false;
			
			auto e_it = m_entities.find(entity);

			// unable to find active entity
			if (e_it == m_entities.end() || e_it->second.empty())
				return false;

			auto meta_id = meta->get_id();
			auto c_pair = std::find_if(e_it->second.begin(), e_it->second.end(), [meta_id](const ComponentPair& comp_pair) {
				return meta_id == comp_pair.first;
			});

			return c_pair != e_it->second.end();
		}

		template <class... IComponents>
		bool has_components(const Entity& entity) const
		{
			auto metas = get_list_types<IComponents...>();
			bool result = true;
			for (MetaClass* meta : metas)
			{
				if (meta == nullptr || !has_component(entity, meta))
				{
					result = false;
					break;
				}
			}
			return result;
		}

		template <class... IComponents>
		std::tuple<IComponents*...> get_components(const Entity& entity)
		{
			return std::make_tuple(get_component<IComponents>(entity)...);
		}

		template <class... IComponents>
		std::vector<std::tuple<IComponents*...>> get_entities_with_components()
		{
			std::vector<std::tuple<IComponents*...>> entities;
			for (auto& entity : m_entities)
			{
				if (has_components<IComponents...>(entity.first))
				{
					entities.push_back(get_components<IComponents...>(entity.first));
				}
			}
			return entities;
		}

		template <class T>
		T* add_system()
		{
			T* ptr = new T();
			m_systems.emplace_back(ptr);
			return ptr;
		}
	};
}