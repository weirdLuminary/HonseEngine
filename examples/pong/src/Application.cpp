#include "Application.h"
#include <honse/modules/components/Renderable.hpp>
#include <stb_image.h>

Pong::Pong() {}
Pong::~Pong() {}


void Pong::Start() {

    Resource<hs::Texture> tex = hs::ResourceManager::Load<hs::Texture>("container", "res/container2.png");


    std::cout << tex->width << '\n';

    std::unique_ptr<hs::Scene> scene = std::make_unique<hs::Scene>();
    Actor a = scene->GetWorld().CreateActor();
    a.AddComponent<hs::Transform>({ { 0.0f, 50.0f }, 0, { 1.0f, 1.0f } });
    a.AddComponent<hs::Renderable>({ tex });
    hs::SceneManager::Load(std::move(scene));
    

}