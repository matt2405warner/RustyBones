#include <iostream>

#include "Core.Engine.h"
#include "./Meta/meta.generated.h"
#include "./Test/Test.hpp"
#include "./Test/MovementComponent.hpp"
#include "./Test/GraphicsComponent.hpp"
#include "./Test/TestSystem.h"

int main()
{
	std::cout << "hello world" << std::endl;

	rusty::InitMeta();
	auto meta = rusty::MetaDB::instance().get_meta("Test");
	if (meta != nullptr)
	{
		std::cout << "found meta for Test" << std::endl;
		auto test = rusty::MetaDB::Create<Test>();
		if (test != nullptr)
		{
			std::cout << "created test 1" << std::endl;
			if (test->get_meta() != nullptr)
			{
				std::cout << "test 1 has meta" << std::endl;
				std::cout << "meta name: " << test->get_meta()->get_name() << std::endl;
			}
		}

		auto test2 = rusty::MetaDB::Create("Test");
		if (test2 != nullptr)
		{
			std::cout << "created test 2" << std::endl;
			if (test2->get_meta() != nullptr)
			{
				std::cout << "test2 has meta" << std::endl;
				std::cout << "meta name: " << test2->get_meta()->get_name() << std::endl;
			}
		}

		auto test3_meta = rusty::get_type<Test>();
		if (test3_meta != nullptr)
		{
			std::cout << "found type from test 3" << std::endl;
			auto test3 = rusty::MetaDB::Create(test3_meta);
			if (test3 != nullptr)
			{
				std::cout << "created test 3" << std::endl;
				if (test3->get_meta() != nullptr)
				{
					std::cout << "test 3 has meta" << std::endl;
					std::cout << "meta name: " << test3->get_meta()->get_name() << std::endl;
				}
			}
		}
	}

	auto manager = rusty::World::instance().get_or_create_instance<rusty::EntityManager>();
	std::cout << "Create and destroy" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		auto entity = manager->create_entity();
		std::cout << "Created Entity: " << entity << "\n";
		manager->destroy_entity(entity);
		std::cout << "Destroyed Entity\n";
	}

	std::cout << "Create only" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		auto entity = manager->create_entity();
		std::cout << "Created Entity: " << entity << "\n";
		if (i >= 5)
		{
			manager->destroy_entity(entity);
			std::cout << "Destroyed Entity\n";
		}
	}

	auto entity = manager->create_entity();
	auto comp = manager->add_component<MovementComponent>(entity);
	comp->x = 10; comp->y = 20; comp->z = 30;

	std::cout << "Movement Component" << std::endl;
	std::cout << "X: " << comp->x << " Y: " << comp->y << " Z: " << comp->z << std::endl;

	auto comp2 = manager->add_component<GraphicsComponent>(entity);
	comp2->shader_name = "Shader Hello!";

	auto get_comp = manager->get_component<MovementComponent>(entity);
	if (get_comp)
	{
		std::cout << "Get Movement Component" << std::endl;
		std::cout << "X: " << get_comp->x << " Y: " << get_comp->y << " Z: " << get_comp->z << std::endl;
	}

	auto entity2 = manager->create_entity();
	auto comp3 = manager->add_component<GraphicsComponent>(entity2);
	comp3->shader_name = "Other Component";

	std::cout << "\nEntities With Components:\n";
	for (auto& [movement, graphics] : manager->get_entities_with_components<MovementComponent, GraphicsComponent>())
	{
		std::cout << "Movement Component" << std::endl;
		std::cout << "X: " << movement->x << " Y: " << movement->y << " Z: " << movement->z << std::endl;
		std::cout << "Graphics Component" << std::endl;
		std::cout << "Name: " << graphics->shader_name << std::endl;
	}

	std::cout << "\nSystem Check\n" << std::endl;
	manager->add_system<TestSystem>();
	
	while (true)
	{
		manager->update(0.0f);
	}

	return 0;
}