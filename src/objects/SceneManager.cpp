#include <honse/objects/SceneManager.h>
#include <honse/objects/Scene.h>
#include <honse/modules/components/Renderable.hpp>
#include <honse/modules/profiling/ScopedTimer.h>
#include <honse/modules/Threading.h>

std::vector<std::unique_ptr<honse::Scene>>
    honse::SceneManager::m_ActiveScenes;

void honse::SceneManager::Update() {
    ScopedTimer timer("ECS update time");
    for (std::unique_ptr<Scene>& scene : m_ActiveScenes) {
        scene->GetWorld().Update();
    }
}

honse::Scene& honse::SceneManager::Load(std::unique_ptr<Scene> scene) {
    Scene& ref = *scene;
    m_ActiveScenes.push_back(std::move(scene));
    return ref;
}

honse::Scene& honse::SceneManager::CreateScene() {
    m_ActiveScenes.push_back(std::make_unique<Scene>());
    return *m_ActiveScenes.back();
}

const std::vector<std::unique_ptr<honse::Scene>>& honse::SceneManager::GetScenes() {
    return m_ActiveScenes;
}