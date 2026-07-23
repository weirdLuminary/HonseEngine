#include <lecs/World.h>
#include <lecs/Actor.h>

World::World() {
    m_Entities = std::make_unique<EntityManager>();
    m_Components = std::make_unique<ComponentManager>();
    m_Systems = std::make_unique<SystemManager>();
}

Entity World::CreateEntity() {
    return m_Entities->CreateEntity();
}

Actor World::CreateActor() {
    return Actor(this);
}

void World::Update() {
    m_Systems->Update(*this);
}