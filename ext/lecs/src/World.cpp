#include "World.h"

World::World() {
    m_Entities = std::make_unique<EntityManager>();
    m_Components = std::make_unique<ComponentManager>();
    m_Systems = std::make_unique<SystemManager>();
}

Entity World::CreateEntity() {
    return m_Entities->CreateEntity();
}