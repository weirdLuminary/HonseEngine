#pragma once
#include "ComponentManager.hpp"
#include "EntityManager.h"
#include "SystemManager.h"
#include <memory>

struct Registry {
    std::shared_ptr<EntityManager> entities;
    std::shared_ptr<ComponentManager> components;
    std::shared_ptr<SystemManager> systems;
};