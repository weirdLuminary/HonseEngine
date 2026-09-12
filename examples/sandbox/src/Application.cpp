#include "Application.h"
#include <honse/components/AtlasRenderable.hpp>
#include <honse/components/Renderable.hpp>
#include <honse/utilities/Math.hpp>
#include <algorithm> 
#include <honse/utilities/Time.h>
#include <honse/audio/Bank.h>
#include <honse/audio/Audio.h>
#include <honse/graphics/Shader.h>
#include <honse/resources/LumPack.hpp>
#include <cstdlib>

Sandbox::~Sandbox() {}

class CameraMovement : public lecs::System {

public:

    void Init(lecs::World& world) override {}

    void Update(lecs::World& world) override {
        const float speed = 500.0f;

        glm::vec2 velocity { 0.0f, 0.0f };

        if(hs::Input::IsKeyDown(hs::Input::Key::W)) velocity.y += 1.0f;
        if(hs::Input::IsKeyDown(hs::Input::Key::A)) velocity.x -= 1.0f;
        if(hs::Input::IsKeyDown(hs::Input::Key::S)) velocity.y -= 1.0f;
        if(hs::Input::IsKeyDown(hs::Input::Key::D)) velocity.x += 1.0f;

        if (glm::length(velocity) <= 0.0f) return;
        hs::Camera::GetMainCamera()->position += glm::normalize(velocity) * hs::Time::GetDeltaTime() * speed;
    }
};

class RotationSystem : public lecs::System {

public:

    void Init(lecs::World& world) override {}

    void Update(lecs::World& world) override {
        lecs::View view = world.GetView<hs::components::Transform>();
        static int iteration = 0;

        for(auto [entity, transform] : view) {
            transform.rotationRadians += glm::radians(0.5f);
        }

        iteration++;
    }
};

void Sandbox::Main() {
    hs::LumPacks::Mount("pack.lum");

    Resource<hs::Texture> tex = hs::ResourceManager::Load<hs::Texture>("john", hs::LumPacks::Path("res/shitsheet.png"));

    Resource<hs::components::AtlasAnimation> anim = hs::ResourceManager::Construct<hs::components::AtlasAnimation>("anim", []{
        Resource<hs::components::AtlasAnimation> res = std::make_shared<hs::components::AtlasAnimation>((hs::components::AtlasAnimation){ { 
            { { 0.0f, 0.0f }, { 15.0f, 15.0f } },
            { { 15.0f, 0.0f }, { 15.0f, 15.0f } },
        }, 2.367f });
        return res;
    });

    auto& scene = hs::SceneManager::CreateScene();

    scene.GetWorld().RegisterSystem<CameraMovement>();
    //scene.GetWorld().RegisterSystem<RotationSystem>();

    for(int i = 0; i < 500; i++) {
        for(int j = 0; j < 19; j++) {
            lecs::Actor a = scene.GetWorld().CreateActor();
            a.AddComponent<hs::components::Transform>({ { i * 64.0f, j * 60.0f }, 0, { 4.0f, 4.0f }, { 0.5f, 0.5f } });
            a.AddComponent<hs::components::AnimatedAtlasRenderable>({ tex, anim, (i+j)%2==0 });
        }
    }
    
}