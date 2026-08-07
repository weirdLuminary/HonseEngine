#pragma once
#include "View.hpp"
#include "Registry.h"

class Actor; 

class World {

public:

    World();

    Entity CreateEntity();
    Actor CreateActor();

    template<typename T>
    void AddComponent(Entity ent, T component)
    {
        m_Registry.components->GetComponentArray<T>()->Add(ent, component);

        Signature newSignature = m_Registry.entities->GetSignature(ent);
        newSignature.set(m_Registry.components->GetComponentType<T>(), true);

        m_Registry.entities->SetSignature(ent, newSignature);
    }

    template<typename SystemType>
    void RegisterSystem() {
        m_Registry.systems->RegisterSystem<SystemType>(*this);
    }
    
    template<typename T>
    void RemoveComponent(Entity ent) {

        Signature newSignature = m_Registry.entities->GetSignature(ent).set(m_Registry.components->GetComponentType<T>(), false);

        m_Registry.components->DestroyComponent<T>(ent);

        m_Registry.entities->SetSignature(ent, newSignature);

    }

    void Update();

    template<typename... Components>
    View<Components...> GetView() {
        return View<Components...>(&m_Registry);
    }

private:

    Registry m_Registry;

    template<typename...>
    friend class View;
};