#include <lecs/World.h>
#include <lecs/Actor.h>

World::World() {
    m_Registry = {
        std::make_shared<EntityManager>(), 
        std::make_shared<ComponentManager>(),
        std::make_shared<SystemManager>()
    };
}

Actor World::CreateActor() {
    return Actor(this);
}

void World::Queue(std::function<void()> func) {
    if(m_IsDeferred) 
        m_DeferredFunctions.push_back(func);
    else 
        func();
}

Entity World::CreateEntity() {
    return m_Registry.entities->CreateEntity();
}

void World::DestroyActor(Entity id) {
    Queue([this, id]() {
        m_Registry.entities->DestroyEntity(id);
        m_Registry.components->OnEntityDestroyed(id);
    });
}

void World::StartFrame() {
    m_IsDeferred = true;
}

void World::FlushDeferred() {
    
    auto commands = std::move(m_DeferredFunctions);
    m_DeferredFunctions.clear();

    m_IsDeferred = false;

    for (auto& command : commands) {
        command();
    }
}

void World::Update() {
    std::cout << "WORLD UPDATE BEGIN\n";
    m_Registry.systems->Update(*this);
    std::cout << "WORLD UPDATE END\n";
}