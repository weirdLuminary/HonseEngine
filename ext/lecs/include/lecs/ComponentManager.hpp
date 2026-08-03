#pragma once
#include "Entity.h"
#include <unordered_map>
#include <memory>
#include "ComponentArray.hpp"
#include <cassert>

class ComponentManager {

public:

    template<typename T>
    void RegisterComponent() {

        const char* name = typeid(T).name();

        m_ComponentTypes.insert({ name, m_NextType });
        m_ComponentArrays.insert({ name, std::make_shared<ComponentArray<T>>() });

        m_NextType++;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        const char* name = typeid(T).name();

        auto it = m_ComponentTypes.find(name);
        if (it == m_ComponentTypes.end())
        {
            RegisterComponent<T>();
            it = m_ComponentTypes.find(name);
        }

        return it->second;
    }

    template<typename T>
    T& GetComponent(Entity ent) {
        return GetComponentArray<T>()->Get(ent);
    }

    template<typename T>
    void AddComponent(Entity ent, T component) {
        GetComponentArray<T>()->Add(ent, component);
    }

    template<typename T>
    void DestroyComponent(Entity ent) {
        GetComponentArray<T>()->Remove(ent);
    }

private:

    std::unordered_map<const char*, ComponentType> m_ComponentTypes;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;

    ComponentType m_NextType = 0;

    template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		if(m_ComponentArrays.find(typeName) == m_ComponentArrays.end()) {
            RegisterComponent<T>();
        }
		return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
	}

    friend class World;
};