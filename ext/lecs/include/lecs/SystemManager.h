#pragma once
#include "Entity.h"
#include "ComponentManager.hpp"
#include <set>
#include <memory>
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

    template<typename T, typename... Components>
    std::shared_ptr<T> RegisterSystem(ComponentManager& cm, World& world) {
        const char* name = typeid(T).name();

        auto system = std::make_shared<T>();

		Signature signature;
    	(signature.set(cm.GetComponentType<Components>()), ...);

		m_Systems.insert({ name, system });
    	m_Signatures.insert({ name, signature });

        system->Init(world);

        return system;
    }

	void OnEntityDestroyed(Entity entity);
	void OnSignatureChanged(Entity entity, Signature entitySignature);

	void Update(World& world);

private:

    std::unordered_map<const char*, std::shared_ptr<System>> m_Systems;
    std::unordered_map<const char*, Signature> m_Signatures;

};