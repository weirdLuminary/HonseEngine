#include <lecs/SystemManager.h>
#include <lecs/World.h>
#include <honse/profiling/ScopedTimer.h>
#include <honse/utilities/Threading.h>

void lecs::SystemManager::OnEntityDestroyed(lecs::Entity ent) {
    for (auto const& pair : m_Systems) {
        auto const& system = pair.second;

        system->m_Entities.erase(ent);
    }
}

void lecs::SystemManager::Update(lecs::World& world) {
    for (auto const& pair : m_Systems) {
        honse::Threading::Enqueue([&] {
            
            honse::ScopedTimer timer("SystemManager update");
            pair.second->Update(world);

        });
    }
    honse::Threading::Wait();
}