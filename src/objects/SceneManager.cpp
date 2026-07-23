#include <honse/objects/SceneManager.h>
#include <honse/objects/Scene.h>
#include <honse/modules/components/Renderable.hpp>

std::vector<std::unique_ptr<hs::Scene>>
    hs::SceneManager::m_ActiveScenes;

void hs::SceneManager::Update() {
    for (std::unique_ptr<Scene>& scene : m_ActiveScenes) {
        scene->GetWorld().Update();
    }
}

hs::Scene& hs::SceneManager::Load(std::unique_ptr<Scene> scene) {
    Scene& ref = *scene;
    m_ActiveScenes.push_back(std::move(scene));
    return ref;
}

const std::vector<std::unique_ptr<hs::Scene>>& hs::SceneManager::GetScenes() {
    return m_ActiveScenes;
}