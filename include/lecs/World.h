#pragma once
#include "Registry.h"
#include "View.hpp"
#include <functional>
#include <vector>

class Actor;

class World {

  public:
    World();

    Entity CreateEntity();

    Actor CreateActor();
    void DestroyActor(Entity id);

    template <typename T> void AddComponent(Entity ent, T component) {
        Queue([this, ent, component]() {
            m_Registry.components->GetComponentArray<T>()->Add(ent, component);

            Signature newSignature = m_Registry.entities->GetSignature(ent);
            newSignature.set(m_Registry.components->GetComponentType<T>(),
                             true);

            m_Registry.entities->SetSignature(ent, newSignature);
        });
    }

    template <typename SystemType> void RegisterSystem() {
        m_Registry.systems->RegisterSystem<SystemType>(*this);
    }

    template <typename T> void RemoveComponent(Entity ent) {

        Queue([this, ent]() {
            Signature newSignature = m_Registry.entities->GetSignature(ent).set(
                m_Registry.components->GetComponentType<T>(), false);

            m_Registry.components->DestroyComponent<T>(ent);

            m_Registry.entities->SetSignature(ent, newSignature);
        });
    }

    void Update();
    void StartFrame();
    void FlushDeferred();

    template <typename... Components> View<Components...> GetView() {
        return View<Components...>(&m_Registry);
    }

  private:
    void Queue(std::function<void()> function);

    Registry m_Registry;

    bool m_IsDeferred = false;
    std::vector<std::function<void()>> m_DeferredFunctions;

    template <typename...> friend class View;
};