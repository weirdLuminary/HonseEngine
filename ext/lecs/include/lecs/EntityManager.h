#pragma once
#include "Entity.h"
#include <queue>
#include <array>
#include <cstdint>

class EntityManager {

public:

    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity& entity);

    void SetSignature(Entity& entity, Signature signature);
    Signature GetSignature(Entity& entity);

private:

    std::array<Entity, MAX_ENTITIES> m_ActiveEntities;
    std::array<Signature, MAX_ENTITIES> m_Signatures;

    std::queue<Entity> m_AvailableSlots; 

    uint32_t m_ActiveEntityCount {};

};