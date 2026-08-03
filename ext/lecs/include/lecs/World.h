#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"

class Actor; 

class World {

public:

    World();

    Entity CreateEntity();
    Actor CreateActor();

    template<typename T>
    void AddComponent(Entity ent, T component)
    {
        m_Components->AddComponent<T>(ent, component);

        Signature newSignature = m_Entities->GetSignature(ent);
        newSignature.set(m_Components->GetComponentType<T>(), true);

        m_Entities->SetSignature(ent, newSignature);
        m_Systems->OnSignatureChanged(ent, newSignature);
    }

    template<typename SystemType, typename... Components>
    void AssignSystem() {
        m_Systems->RegisterSystem<SystemType, Components...>(*m_Components, *this);
        for(Entity ent = 0; ent < m_Entities->m_ActiveEntityCount; ent++) {
            m_Systems->OnSignatureChanged(m_Entities->m_ActiveEntities[ent], m_Entities->GetSignature(ent));
        }
    }
    
    template<typename T>
    void RemoveComponent(Entity ent) {

        Signature newSignature = m_Entities->GetSignature(ent).set(m_Components->GetComponentType<T>(), false);

        m_Components->DestroyComponent<T>(ent);

        m_Entities->SetSignature(ent, newSignature);
        m_Systems->OnSignatureChanged(ent, newSignature);

    }

    template<typename T>
    T& GetComponent(Entity ent) {
        
        return m_Components->GetComponent<T>(ent);
        
    }

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        return m_Components->GetComponentArray<T>();
    }

    void Update();

private:

    std::unique_ptr<EntityManager> m_Entities;
    std::unique_ptr<ComponentManager> m_Components;
    std::unique_ptr<SystemManager> m_Systems;

};