#pragma once
#include <memory>
#include "EntityManager.h"
#include "ComponentManager.hpp"
#include "SystemManager.h"

struct Registry {
    std::shared_ptr<EntityManager> entities;
    std::shared_ptr<ComponentManager> components;
    std::shared_ptr<SystemManager> systems;
};