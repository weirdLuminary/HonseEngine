#include <honse/objects/Scene.h>
#include <honse/modules/components/Renderable.hpp>

honse::Scene::Scene() {
    m_World = World();
    m_World.AssignSystem<honse::RenderableHandler, honse::Renderable, honse::Transform>();
}

World& honse::Scene::GetWorld() {
    return m_World;
}