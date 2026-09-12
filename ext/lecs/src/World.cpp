#include <lecs/World.h>
#include <lecs/Actor.h>

lecs::World::World() {
    m_Registry = {
        std::make_shared<EntityManager>(), 
        std::make_shared<ComponentManager>(),
        std::make_shared<SystemManager>()
    };
}

lecs::Actor lecs::World::CreateActor() {
    return lecs::Actor(this);
}

void lecs::World::Queue(std::function<void()> func) {
    if(m_IsDeferred) 
        m_DeferredFunctions.push_back(func);
    else 
        func();
}

lecs::Entity lecs::World::CreateEntity() {
    return m_Registry.entities->CreateEntity();
}

void lecs::World::DestroyActor(Entity id) {
    Queue([this, id]() {
        m_Registry.entities->DestroyEntity(id);
        m_Registry.components->OnEntityDestroyed(id);
    });
}

void lecs::World::StartFrame() {
    m_IsDeferred = true;
}

void lecs::World::FlushDeferred() {
    
    auto commands = std::move(m_DeferredFunctions);
    m_DeferredFunctions.clear();

    m_IsDeferred = false;

    for (auto& command : commands) {
        command();
    }
}

void lecs::World::Update() {
    std::cout << "WORLD UPDATE BEGIN\n";
    m_Registry.systems->Update(*this);
    std::cout << "WORLD UPDATE END\n";
}