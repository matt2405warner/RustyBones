#pragma once

#include <string>
//#include "GuidGenerator.hpp"
#include "Test\GraphicsComponent.hpp"
#include "Test\MovementComponent.hpp"
#include "Test\Test.hpp"

namespace rusty
{
	// Construct MetaData info for GraphicsComponent
	template <>
	MetaClass* construct_type<GraphicsComponent>()
	{
		static_assert(std::is_base_of_v<Object, GraphicsComponent>, "Cannot construct meta info for GraphicsComponent if it does not derive from Object");

		auto generated = new MetaClass(
			0,
			std::string("GraphicsComponent"),
			sizeof(GraphicsComponent),
			[]() -> Object* { return static_cast<Object*>(new GraphicsComponent()); },
			[](unsigned char* address) -> Object* { return static_cast<Object*>(new (address) GraphicsComponent()); },
			[](Object* obj) { auto cls = reinterpret_cast<GraphicsComponent*>(obj); cls->~GraphicsComponent(); }
		);

		// Generate class members
		/*		generated->add_member("shader_name", OffsetOf(&GraphicsComponent::shader_name));
		// Generate class functions
*/

		MetaDB::instance().add_type(generated);

		return generated;
	}

	// Get meta data info for GraphicsComponent.
	// if nullptr then construct meta info for GraphicsComponent.
	template <>
	MetaClass* get_type<GraphicsComponent>()
	{
		static_assert(std::is_base_of_v<Object, GraphicsComponent>, "Cannot get meta info for GraphicsComponent if it does not derive from Object");

		auto generated = MetaDB::instance().get_meta("GraphicsComponent");

		if (generated != nullptr)
			return generated;

		generated = construct_type<GraphicsComponent>();

		return generated;
	}
	// Construct MetaData info for MovementComponent
	template <>
	MetaClass* construct_type<MovementComponent>()
	{
		static_assert(std::is_base_of_v<Object, MovementComponent>, "Cannot construct meta info for MovementComponent if it does not derive from Object");

		auto generated = new MetaClass(
			1,
			std::string("MovementComponent"),
			sizeof(MovementComponent),
			[]() -> Object* { return static_cast<Object*>(new MovementComponent()); },
			[](unsigned char* address) -> Object* { return static_cast<Object*>(new (address) MovementComponent()); },
			[](Object* obj) { auto cls = reinterpret_cast<MovementComponent*>(obj); cls->~MovementComponent(); }
		);

		// Generate class members
		/*		generated->add_member("x", OffsetOf(&MovementComponent::x));
		generated->add_member("y", OffsetOf(&MovementComponent::y));
		generated->add_member("z", OffsetOf(&MovementComponent::z));
		generated->add_member("speed", OffsetOf(&MovementComponent::speed));
		// Generate class functions
*/

		MetaDB::instance().add_type(generated);

		return generated;
	}

	// Get meta data info for MovementComponent.
	// if nullptr then construct meta info for MovementComponent.
	template <>
	MetaClass* get_type<MovementComponent>()
	{
		static_assert(std::is_base_of_v<Object, MovementComponent>, "Cannot get meta info for MovementComponent if it does not derive from Object");

		auto generated = MetaDB::instance().get_meta("MovementComponent");

		if (generated != nullptr)
			return generated;

		generated = construct_type<MovementComponent>();

		return generated;
	}
	// Construct MetaData info for Test
	template <>
	MetaClass* construct_type<Test>()
	{
		static_assert(std::is_base_of_v<Object, Test>, "Cannot construct meta info for Test if it does not derive from Object");

		auto generated = new MetaClass(
			2,
			std::string("Test"),
			sizeof(Test),
			[]() -> Object* { return static_cast<Object*>(new Test()); },
			[](unsigned char* address) -> Object* { return static_cast<Object*>(new (address) Test()); },
			[](Object* obj) { auto cls = reinterpret_cast<Test*>(obj); cls->~Test(); }
		);

		// Generate class members
		/*		// Generate class functions
*/

		MetaDB::instance().add_type(generated);

		return generated;
	}

	// Get meta data info for Test.
	// if nullptr then construct meta info for Test.
	template <>
	MetaClass* get_type<Test>()
	{
		static_assert(std::is_base_of_v<Object, Test>, "Cannot get meta info for Test if it does not derive from Object");

		auto generated = MetaDB::instance().get_meta("Test");

		if (generated != nullptr)
			return generated;

		generated = construct_type<Test>();

		return generated;
	}

	static void InitMeta()
	{
		auto init_1 = construct_type<GraphicsComponent>();
		auto init_2 = construct_type<MovementComponent>();
		auto init_3 = construct_type<Test>();
	}
}