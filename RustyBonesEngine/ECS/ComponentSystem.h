#pragma once

namespace rusty
{
	class IComponentSystem
	{
		friend class EntityManager;
	protected:
		virtual void update_system(float dt) = 0;
	};

	template <class C, class... T>
	class ComponentSystem : public IComponentSystem
	{
	public:
		//template <class CW_T>
		//using CW = rusty::ComponentWrapper<CW_T>;

	private:

		template <class TupleT, std::size_t... I>
		void invoke_update(float dt, const TupleT& args, std::index_sequence<I...>)
		{
			static_cast<C*>(this)->update(dt, std::get<I>(args)...);
		}

	protected:
		void update_system(float dt) override final
		{
			auto em = World::instance().get_or_create_instance<EntityManager>();
			if (em == nullptr)
				return;

			auto tuple_vec = em->get_entities_with_components<T...>();
			for (auto& t : tuple_vec)
			{
				invoke_update(dt, t, std::index_sequence_for<T...>{});
			}
		}
	};
}