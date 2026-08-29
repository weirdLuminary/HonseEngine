#pragma once
#include "Entity.h"
#include <array>
#include <cstdint>
#include <queue>
#include <vector>

class EntityManager {

  public:
    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

  private:
    std::vector<Entity> m_ActiveEntities;
    std::array<Signature, MAX_ENTITIES> m_Signatures;

    std::queue<Entity> m_AvailableSlots;

    uint32_t m_ActiveEntityCount{};

    friend class World;

    template <typename...> friend class View;
};