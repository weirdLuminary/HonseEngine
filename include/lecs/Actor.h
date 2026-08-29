#pragma once
#include "World.h"

// Abstract representation of the ECS entity ID with functions.
class Actor {

  public:
    Actor() = delete;

    template <typename T> void AddComponent(T component) {
        if (!m_Initialized)
            return;

        m_World->AddComponent<T>(m_ID, component);
    };

    template <typename T> void RemoveComponent() {
        if (!m_Initialized)
            return;

        m_World->RemoveComponent<T>(m_ID);
    };

    Entity GetInternalID() { return m_ID; }

  private:
    Actor(World* world);

    World* m_World;
    Entity m_ID;
    bool m_Initialized = false;

    friend class World;
};