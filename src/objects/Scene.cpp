#include <honse/objects/Scene.h>

void hs::Scene::Add(std::unique_ptr<Object> obj) {
    m_Entities.push_back(std::move(obj));
}

const std::vector<std::unique_ptr<hs::Object>>& hs::Scene::GetEntities() const {
    return m_Entities;
}