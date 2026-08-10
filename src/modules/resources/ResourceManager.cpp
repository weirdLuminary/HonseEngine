#include <honse/modules/resources/ResourceManager.h>

std::vector<honse::IResourceCache*> honse::ResourceManager::m_Caches;

void honse::ResourceManager::Shutdown() {
    for(IResourceCache* cache : m_Caches) {
        cache->Clear();
    }
    m_Caches.clear();
}