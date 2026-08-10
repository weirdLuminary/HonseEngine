#pragma once
#include <iostream>

namespace honse {

    template<typename T, typename... Args>
    Resource<T> ResourceManager::Load(const std::string& key, Args&&... args)
    {
        printf("Trying to load \'%s\'...\n", key.c_str());
        return GetCache<T>().Load(
            hash(key),
            std::forward<Args>(args)...
        );
    }


    template<typename T, typename Factory>
    Resource<T> ResourceManager::Construct(const std::string& key, Factory&& factory)
    {
        printf("Trying to construct \'%s\'...\n", key.c_str());
        return GetCache<T>().Construct(
            hash(key),
            std::forward<Factory>(factory)
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