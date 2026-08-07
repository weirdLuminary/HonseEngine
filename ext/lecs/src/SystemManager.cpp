#include <lecs/SystemManager.h>
#include <lecs/World.h>
#include <honse/modules/profiling/ScopedTimer.h>
#include <honse/modules/Threading.h>

void SystemManager::OnEntityDestroyed(Entity ent) {
    for (auto const& pair : m_Systems) {
        auto const& system = pair.second;

        system->m_Entities.erase(ent);
    }
}

void SystemManager::Update(World& world) {
    for (auto const& pair : m_Systems) {
        honse::Threading::Enqueue([&] {
            
            honse::ScopedTimer timer("SystemManager update");
            pair.second->Update(world);

        });
    }
    honse::Threading::Wait();
}