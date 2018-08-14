#pragma once

#include <vector>
#include "../Core/Util/Singleton.h"
//#include "Object.h"
//#include "MetaClass.h"

template <typename T, typename U>
std::size_t OffsetOf(U T::*member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

namespace rusty
{
	template <class T>
	MetaClass* get_type()
	{
		return nullptr;
	}

	template <class T>
	MetaClass* construct_type()
	{
		return nullptr;
	}

	class MetaDB : public Singleton<MetaDB>
	{
	private:
		std::vector<MetaClass*> m_types;

	public:
		MetaClass * get_meta(const std::string& name);
		MetaClass* get_meta(std::size_t id);
		void add_type(MetaClass* meta);
		static Object* Create(MetaClass* meta)
		{
			if (meta == nullptr)
				return nullptr;

			Object* obj = meta->m_ctor();
			obj->m_meta = meta;
			return obj;
		}

		static Object* Create(MetaClass* meta, unsigned char* address)
		{
			if (meta == nullptr)
				return nullptr;

			Object* obj = meta->m_ctor_inplace(address);
			obj->m_meta = meta;
			return obj;
		}

		static Object* Create(const std::string& name)
		{
			auto meta = instance().get_meta(name);

			if (meta == nullptr)
				return nullptr;

			auto obj = meta->m_ctor();
			obj->m_meta = meta;
			return obj;
		}

		static Object* Create(const std::string& name, unsigned char* address)
		{
			auto meta = instance().get_meta(name);

			if (meta == nullptr)
				return nullptr;

			auto obj = meta->m_ctor_inplace(address);
			obj->m_meta = meta;
			return obj;
		}

		template <class T>
		static T* Create()
		{
			static_assert(std::is_base_of_v<Object, T>, "Cannot create instance that does not derive from Object");

			MetaClass* meta = get_type<T>();

			if (meta == nullptr)
				return nullptr;

			auto obj = meta->m_ctor();
			obj->m_meta = meta;
			return static_cast<T*>(obj);
		}

		template <class T>
		static T* Create(unsigned char* address)
		{
			static_assert(std::is_base_of_v<Object, T>, "Cannot create instance that does not derive from Object");

			MetaClass* meta = get_type<T>();

			if (meta == nullptr)
				return nullptr;

			auto obj = meta->m_ctor_inplace(address);
			obj->m_meta = meta;
			return static_cast<T*>(obj);
		}
	};
}