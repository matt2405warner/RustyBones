#pragma once

namespace rusty
{
	class MetaClass;

	class Object
	{
		friend class MetaDB;

	private:
		MetaClass * m_meta;

	public:
		virtual ~Object() = default;
		MetaClass * const get_meta() const;
	};
}