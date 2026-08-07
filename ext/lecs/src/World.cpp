#include <lecs/World.h>
#include <lecs/Actor.h>

World::World() {
    m_Registry = {
        std::make_shared<EntityManager>(), 
        std::make_shared<ComponentManager>(),
        std::make_shared<SystemManager>()
    };
}

Entity World::CreateEntity() {
    return m_Registry.entities->CreateEntity();
}

Actor World::CreateActor() {
    return Actor(this);
}

void World::Update() {
    m_Registry.systems->Update(*this);
}