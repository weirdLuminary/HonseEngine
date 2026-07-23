#include <lecs/SystemManager.h>
#include <lecs/World.h>

void SystemManager::OnEntityDestroyed(Entity ent) {
    for (auto const& pair : m_Systems) {
        auto const& system = pair.second;

        system->m_Entities.erase(ent);
    }
}

void SystemManager::OnSignatureChanged(Entity entity, Signature entitySignature) {
    for (auto const& pair : m_Systems) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = m_Signatures[type];
        std::cout << "S > " << systemSignature << '\n';

        if ((entitySignature & systemSignature) == systemSignature) {
            system->m_Entities.insert(entity);
            std::cout << "MATCH" << '\n';
        }
        else {
            system->m_Entities.erase(entity);
        }
    }
}

void SystemManager::Update(World& world) {
    for (auto const& pair : m_Systems) {
        for(Entity e : pair.second->m_Entities) {
            pair.second->Update(e, world);
        }
    }
}