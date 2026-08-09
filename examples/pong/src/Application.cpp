#include "Application.h"
#include <honse/modules/components/Renderable.hpp>
#include <honse/modules/Math.hpp>
#include <algorithm> 
#include <honse/modules/Time.h>
#include <honse/audio/Bank.h>
#include <honse/audio/Audio.h>

Pong::Pong() {}
Pong::~Pong() {}

class CameraMovement : public System {

public:

    void Init(World& world) override {}

    void Update(World& world) override {
        static float speed = 500.0f;

        glm::vec2 velocity { 0.0f, 0.0f };

        if(hs::Input::IsKeyDown(hs::Input::Key::W)) velocity.y += 1.0f;
        if(hs::Input::IsKeyDown(hs::Input::Key::A)) velocity.x -= 1.0f;
        if(hs::Input::IsKeyDown(hs::Input::Key::S)) velocity.y -= 1.0f;
        if(hs::Input::IsKeyDown(hs::Input::Key::D)) velocity.x += 1.0f;

        if (glm::length(velocity) <= 0.0f) return;
        hs::Camera::GetMainCamera()->position += glm::normalize(velocity) * hs::Time::GetDeltaTime() * speed;
    }
};

class RotationSystem : public System {

public:

    void Init(World& world) override {}

    void Update(World& world) override {
        View view = world.GetView<hs::Transform>();

        for(auto [entity, transform] : view) {
            transform.rotation += 0.5f;
        }
    }
};

void Pong::Main() {

    Resource<hs::Texture> tex = hs::ResourceManager::Load<hs::Texture>("john", "res/johnthepain.png");

    auto& scene = hs::SceneManager::CreateScene();

    Resource<hs::Bank> bank = hs::Audio::LoadBank("master", "res/Master.bank");

    scene.GetWorld().RegisterSystem<CameraMovement>();
    scene.GetWorld().RegisterSystem<RotationSystem>();

    for(int i = 0; i < 500; i++) {
        for(int j = 0; j < 19; j++) {
            Actor a = scene.GetWorld().CreateActor();
            a.AddComponent<hs::Transform>({ { i * 10.0f, 50.0f * j }, i * (float)j, { 1.0f, 1.0f }, { 0.5f, 0.5f } });
            a.AddComponent<hs::Renderable>({ tex });
        }
    }
    
}