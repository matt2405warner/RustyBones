# RustyBones - C++ Game Engine built using Entity Component System
RustyBones is a Game Engine built with a Entity Component System architecture. 

# Downloading
In your favourite terminal you can check out the current development progress with:
```
git clone https://github.com/matt2405warner/RustyBones.git
```

# Contact
You can contact me on my [Gitter Channel](https://gitter.im/Matt-s-Projects/Lobby).

You can also contact me via [email](mailto:mattwarner@rogers.com).

# Overview
In RustyBones entities only hold an index and version number to provide a unique-id. Data is presented in the form of `rusty::IComponent`. These components are symbolically linked together internally in the `rusty::EntityManager`. Components are stored in a cache friendly manner in which the `rusty::EntityManager` controls creation and destruction of components, entities, and systems. `rusty::ComponentSystem` holds the logic for similar entities.

### Entities
Under the hood Entities have two numbers `id` and `version` which provide a mechanism for unique numbers in an effiecent manner. Entities are the glue that holds components together to provide a set of behaviour which is defined through the implementation of systems.

Creating an Entity:
```cpp
rusty::EntityManager* manager = rusty::World::instance().get_or_create_instance<rusty::EntityManager>();
rusty::Entity entity = manager->create_entity();
```

```cpp
manager->destroy_entity(entity);
```

**Implementation Details:**
* When a entity is destroyed its `id` is added to a free list inside of `rusty::EntityManager`.
* When a entity is created it first checks if there is an enity in the free list located in the `rusty::EntityManager`. If so, then the version is incremented for this specific entity otherwise the version is one and the id is the next largest id created.

### Components (Holds only Data)
Components in RustyBones should only contain data and as little logic as possible. This is to provide the best possible cache friendliness. All logic should be contained inside of `rusty::Systems`.

*Creating Components*
An example component might be a movement componented represented as:
```cpp
class Movement : IComponent
{
public:
  float x, y, z;
  float speed;
};

class Position : IComponent
{
public:
  float x, y, z;
};
```

**Adding components to their respective entities**
Associating a component with a previously created entity is as simple as:
```cpp
Movement* movement = manager->add_component<Movement>(entity);
```

**Quering components**
To retrieve a singular component use `manager->get_component<T>(entity);`
```cpp
Movement* movement = manager->get_component<Movement>(entity);
```

To retrieve multiple components associated with an enity use `manager->get_components<T, ...Tn>(entity);`
```cpp
std::tuple<Movement*, Position*> components = manager->get_components<Movement, Position>(entity);
```

To retrieve all components with entities that have at least the same signature use:
```cpp
std::vector<std::tuple<Movement, Position>> components = manager->get_entities_with_components<Movement, Position>();
```

To remove components use: `manager->remove_component<T>(entity)`

### Systems
Systems are where you implement behaviour using 1...n components. Implement systems by subclassing `ComponentSystem<C, ...T>`.
**It is required that systems implement the update method**.

**Basic Implementation**
```cpp
class MovementSystem : public ComponentSystem<MovementSystem, Movement, Position>
{
public:
void update(float dt, Movement* movement, Position* position)
{
  position.x += movement->x * movement->speed * dt;
  position.y += movement->y * movement->speed * dt;
  position.z += movement->z * movement->speed * dt;
}
};
```

# Requirements
* Visual Studio 2017 with atleast C++ 17 enabled.
* Libclang python binding installed on build machine.

RustyBones has currently only been tested on Windows with Visual Studio 2017. Reports of patches and bugs are welcomed.
