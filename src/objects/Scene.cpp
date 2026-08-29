#include <honse/modules/components/Renderable.hpp>
#include <honse/objects/Scene.h>

honse::Scene::Scene() {
    m_World = World();
    m_World.RegisterSystem<RenderableHandler>();
}

World& honse::Scene::GetWorld() { return m_World; }