#pragma once

#include <string>
#include <functional>
#include <vector>

namespace rusty
{
	class Object;
	using ObjectCtor = std::function<Object*()>;
	using ObjectCtorInPlace = std::function<Object* (unsigned char* address)>;
	using ObjectDtor = std::function<void(Object* obj)>;
	using MetaId = std::size_t;

	struct MetaMember
	{
		MetaMember(const std::string& name, std::size_t offset);

		const std::string m_name;
		const std::size_t m_offset;
	};

	struct MetaFunction
	{
		MetaFunction(const std::string& name, std::size_t offset);
		const std::string m_name;
		const std::size_t m_offset;
	};

	class MetaClass
	{
		friend class MetaDB;
		template <class T> friend MetaClass* construct_type();

	private:
		std::vector<MetaMember> m_members;
		std::vector<MetaFunction> m_functions;
		ObjectCtor m_ctor;
		ObjectCtorInPlace m_ctor_inplace;
		ObjectDtor m_dtor;
		std::string m_name;
		MetaId m_id;
		std::size_t m_size;

	public:
		MetaClass(MetaId id, const std::string& name, std::size_t size, ObjectCtor ctor, ObjectCtorInPlace inplace, ObjectDtor dtor);
		void destroy(Object* obj) const;

		const std::string& get_name() const;
		const std::size_t get_id() const;
		const std::size_t get_size() const;
	};
}