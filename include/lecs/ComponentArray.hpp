#pragma once
#include "Entity.h"
#include <iostream>
#include <array>

class IComponentArray {

public:
    virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;

};

template<typename T>
class ComponentArray : public IComponentArray {

public:


    void Add(Entity ent, T component) {

        m_EntityToIndex[ent] = m_Size;
        m_IndexToEntity[m_Size] = ent;

        m_Components[m_Size] = component;

        m_Size++;

        //std::cout << "++ Component " << typeid(T).name() << " Ent " << ent << '\n';
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

        m_EntityToIndex[ent] = 0;
        m_IndexToEntity[lastIndex] = 0;

        m_Size--;

        //std::cout << "-- Component " << typeid(T).name() << " Ent " << ent << " MOVE from " << lastEntity << '\n';
    }

    T& Get(Entity ent) {
        return m_Components[m_EntityToIndex[ent]];
    }

    T& Get(size_t index)
    {
        return m_Components[index];
    }

    Entity GetEntity(size_t index) const
    {
        return m_IndexToEntity[index];
    }

    size_t Size() const
    {
        return m_Size;
    }

    void EntityDestroyed(Entity entity) override {
        Remove(entity);
    };

private:

    std::array<size_t, MAX_ENTITIES> m_EntityToIndex;
    std::array<Entity, MAX_ENTITIES> m_IndexToEntity;

    std::array<T, MAX_ENTITIES> m_Components;

    // Total size of valid entries in the array.
    size_t m_Size = 0;

};