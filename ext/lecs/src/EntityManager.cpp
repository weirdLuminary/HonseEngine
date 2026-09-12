#include <lecs/EntityManager.h>
#include <iostream>
#include <cassert>
#include <algorithm>

lecs::EntityManager::EntityManager() {
    m_ActiveEntities.reserve(MAX_ENTITIES);
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        m_AvailableSlots.push(entity);
    }
}


lecs::Entity lecs::EntityManager::CreateEntity()
{
    assert(m_ActiveEntityCount < MAX_ENTITIES && "Too many entities in existence.");

    lecs::Entity id = m_AvailableSlots.front();
    m_AvailableSlots.pop();

    m_ActiveEntities.push_back(id);

    m_ActiveEntityCount++;

    //std::cout << "++ Entity " << id << '\n';

    return id;
}

void lecs::EntityManager::DestroyEntity(lecs::Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    m_Signatures[entity].reset();

    m_AvailableSlots.push(entity);

    auto it = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entity);

    if (it != m_ActiveEntities.end())
        m_ActiveEntities.erase(it);

    m_ActiveEntityCount--;

    //std::cout << "-- Entity " << entity << '\n';
}

void lecs::EntityManager::SetSignature(lecs::Entity entity, lecs::Signature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    m_Signatures[entity] = signature;

    //std::cout << "E > " << signature << '\n';
}

lecs::Signature lecs::EntityManager::GetSignature(lecs::Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");

    return m_Signatures[entity];
}
