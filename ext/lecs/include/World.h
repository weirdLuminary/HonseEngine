#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"

class Actor; 

class World {

public:

    World();

    Entity CreateEntity();

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
        m_Systems->RegisterSystem<SystemType, Components...>(*m_Components);
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

    void Update() {
        m_Systems->Update(*this);
    }

private:

    std::unique_ptr<EntityManager> m_Entities;
    std::unique_ptr<ComponentManager> m_Components;
    std::unique_ptr<SystemManager> m_Systems;

};