#include <honse/objects/SceneManager.h>
#include <honse/objects/Scene.h>
#include <honse/graphics/Sprite.h>
#include <honse/graphics/Renderer.h>

std::vector<std::unique_ptr<hs::Scene>>
    hs::SceneManager::m_ActiveScenes;

void hs::SceneManager::Render() {

    for (std::unique_ptr<Scene>& scene : m_ActiveScenes) {

        for (const std::unique_ptr<Object>& obj : scene->GetEntities()) {

            if (auto* drawable = dynamic_cast<Sprite*>(obj.get())) {
                Renderer::Submit(*drawable);
            }
        }
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