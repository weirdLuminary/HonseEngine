#pragma once
#include "Entity.h"
#include <memory>
#include <set>
#include <unordered_map>

class World;

class System {

  public:
    virtual ~System() = default;
    virtual void Init(World& world) = 0;
    virtual void Update(World& world) = 0;

  private:
    std::set<Entity> m_Entities;

    friend class SystemManager;
};

class SystemManager {

  public:
    template <typename T> std::shared_ptr<T> RegisterSystem(World& world) {

        auto system = std::make_shared<T>();

        m_Systems.insert({typeid(T).name(), system});

        system->Init(world);

        return system;
    }

    void OnEntityDestroyed(Entity entity);

    void Update(World& world);

  private:
    std::unordered_map<const char*, std::shared_ptr<System>> m_Systems;
};