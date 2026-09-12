#include <honse/components/Renderable.hpp>
#include <honse/components/Scene.h>
#include <honse/components/SceneManager.h>
#include <honse/profiling/ScopedTimer.h>
#include <honse/utilities/Threading.h>

std::vector<std::unique_ptr<honse::Scene>> honse::SceneManager::m_ActiveScenes;

void honse::SceneManager::Update() {
    ScopedTimer timer("ECS update time");
    for (std::unique_ptr<Scene>& scene : m_ActiveScenes) {
        scene->GetWorld().Update();
    }
}

void honse::SceneManager::StartFrame() {
    for (std::unique_ptr<Scene>& scene : m_ActiveScenes) {
        scene->GetWorld().StartFrame();
    }
}

void honse::SceneManager::EndFrame() {
    for (std::unique_ptr<Scene>& scene : m_ActiveScenes) {
        scene->GetWorld().FlushDeferred();
    }
}

void honse::SceneManager::Shutdown() { m_ActiveScenes.clear(); }

honse::Scene& honse::SceneManager::Load(std::unique_ptr<Scene> scene) {
    Scene& ref = *scene;
    m_ActiveScenes.push_back(std::move(scene));
    return ref;
}

honse::Scene& honse::SceneManager::CreateScene() {
    m_ActiveScenes.push_back(std::make_unique<Scene>());
    return *m_ActiveScenes.back();
}

const std::vector<std::unique_ptr<honse::Scene>>& honse::SceneManager::GetScenes() { return m_ActiveScenes; }