#pragma once
#include "ComponentArray.hpp"
#include "Registry.h"

template <typename... Components> class View {
  public:
    class Iterator {

      public:
        Iterator(size_t index, View* view) : m_Index(index), m_View(view) {}

        std::tuple<Entity, Components&...> operator*() const {

            Entity entity = std::get<0>(m_View->m_Storages)->GetEntity(m_Index);

            return {entity,
                    std::get<std::shared_ptr<ComponentArray<Components>>>(
                        m_View->m_Storages)
                        ->Get(entity)...};
        }

        Iterator& operator++() {
            ++m_Index;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return m_Index != other.m_Index;
        }

      private:
        size_t m_Index;
        View* m_View;
    };

    template <typename T> T& GetComponent(Entity e) {
        return std::get<std::shared_ptr<ComponentArray<T>>>(m_Storages)->Get(e);
    }

    Iterator begin() { return Iterator{0, this}; }

    Iterator end() { return Iterator{std::get<0>(m_Storages)->Size(), this}; }

  private:
    View(Registry* reg)
        : m_Storages{reg->components->GetComponentArray<Components>()...} {}

    std::tuple<std::shared_ptr<ComponentArray<Components>>...> m_Storages;

    friend class World;
};
