#pragma once
#include <iostream>

namespace honse {

    template<typename T, typename... Args>
    Resource<T> ResourceManager::Load(const std::string& key, Args&&... args)
    {
        auto& cache = GetCache<T>();

        auto hashKey = hash(key);

        return cache.Load(
            hashKey,
            std::forward<Args>(args)...
        );
    }

    template<typename T>
    ResourceCache<T>& ResourceManager::GetCache()
    {
        static ResourceCache<T> cache;

        static bool registered = []()
        {
            m_Caches.push_back(&cache);
            return true;
        }();

        (void)registered;

        return cache;
    };

}