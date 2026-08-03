#pragma once
#include "ComponentArray.hpp"
#include <memory>

template<typename... Components>
class View
{
public:
    View(World& world) : m_Storages{ &world.GetStorage<Components>()... } {}

    template<typename T>
    T& GetComponent(Entity e) {
        return storages
    }

private:
    std::tuple<std::shared_ptr<ComponentArray<Components>>...> m_Storages;
};