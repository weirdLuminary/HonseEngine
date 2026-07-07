#include "Application.h"

Pong::Pong() {}
Pong::~Pong() {}

hs::Texture tex;
hs::Texture texA;

void Pong::Start() {

    tex = hs::Texture("res/container.jpg");
    texA = hs::Texture("res/container2.png");

    auto scene = std::make_unique<hs::Scene>();

    scene->Create<hs::Sprite>(&tex);
    scene->Create<hs::Sprite>(&texA).position = glm::vec2(100.0f);

    hs::SceneManager::Load(std::move(scene));

}

void Pong::Update() {


}