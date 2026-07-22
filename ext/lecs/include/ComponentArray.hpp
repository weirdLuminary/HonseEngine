#pragma once
#include "Entity.h"
#include <unordered_map>
#include <iostream>

class IComponentArray {

public:
    virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;

};

template<typename T>
class ComponentArray : public IComponentArray {

public:


    void Add(Entity ent, T component) {

        m_EntityToIndex.insert({ ent, m_Size });
        m_IndexToEntity.insert({ m_Size, ent });

        m_Components[m_Size] = component;

        m_Size++;

        std::cout << "++ Component " << typeid(T).name() << " Ent " << ent << '\n';
    };

    void Remove(Entity ent) {
        size_t index = m_EntityToIndex[ent]; // Index of the entity to be deleted
        size_t lastIndex = m_Size - 1; // Index of the last entity

        Entity lastEntity = m_IndexToEntity[lastIndex];


        // Move last index to deleted entity index to keep the array packed

        m_Components[index] = m_Components[lastIndex];
        m_EntityToIndex[lastEntity] = index;
        m_IndexToEntity[index] = lastEntity;


        // Remove extra index

        m_EntityToIndex.erase(ent);
        m_IndexToEntity.erase(lastIndex);;

        m_Size--;

        std::cout << "-- Component " << typeid(T).name() << " Ent " << ent << " MOVE from " << lastEntity << '\n';
    }

    T& Get(Entity ent) {
        return m_Components[m_EntityToIndex[ent]];
    }

    void EntityDestroyed(Entity entity) override {
        Remove(entity);
    };

private:

    // Note: unordered maps are faster even though we're aiming for the data to be properly sorted due to the complexity being O(1) compared to map's O(log n)

    std::unordered_map<Entity, size_t> m_EntityToIndex;
    std::unordered_map<size_t, Entity> m_IndexToEntity;

    std::array<T, MAX_ENTITIES> m_Components;

    // Total size of valid entries in the array.
    size_t m_Size = 0;

};