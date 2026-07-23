#include <honse/objects/Scene.h>
#include <honse/modules/components/Renderable.hpp>

hs::Scene::Scene() {
    m_World = World();
    m_World.AssignSystem<hs::RenderableHandler, hs::Renderable, hs::Transform>();
}

World& hs::Scene::GetWorld() {
    return m_World;
}