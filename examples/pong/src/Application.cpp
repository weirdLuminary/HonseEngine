#include "Application.h"
#include <honse/modules/components/Renderable.hpp>
#include <honse/modules/Math.hpp>
#include <algorithm> 

Pong::Pong() {}
Pong::~Pong() {}


void Pong::Start() {

    Resource<hs::Texture> tex = hs::ResourceManager::Load<hs::Texture>("john", "res/johnthepain.png");

    auto& scene = hs::SceneManager::CreateScene();

    for(int i = 0; i < 500; i++) {
        for(int j = 0; j < 19; j++) {
            Actor a = scene.GetWorld().CreateActor();
            a.AddComponent<hs::Transform>({ { i * 10.0f, 50.0f * j }, i * (float)j, { 1.0f, 1.0f }, { 0.5f, 0.5f } });
            a.AddComponent<hs::Renderable>({ tex });
        }
    }
    
}