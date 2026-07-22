#pragma once
#include "World.h"

// Simple ECS wrapper
class Actor {

public:

    Actor(World* world);
    //~Actor();
    
    template<typename T>
    void AddComponent(T component) {
        if(!m_Initialized) return;

        m_World->AddComponent<T>(m_ID, component);
    };

    template<typename T>
    void RemoveComponent() {
        if(!m_Initialized) return;

        m_World->RemoveComponent<T>(m_ID);
    };

    template<typename T>
    T& GetComponent() { 
        return m_World->GetComponent<T>(m_ID);
    };

    Entity GetInternalID() { return m_ID; }

private:

    World* m_World;
    Entity m_ID;
    bool m_Initialized = false;


};