#include <honse/components/AtlasRenderable.hpp>
#include <honse/components/Renderable.hpp>
#include <honse/components/Scene.h>

honse::Scene::Scene() {

    m_World = lecs::World();

    // Register default engine systems
    m_World.RegisterSystem<components::RenderableHandler>();
    m_World.RegisterSystem<components::AtlasRenderableHandler>();
}

lecs::World& honse::Scene::GetWorld() { return m_World; }