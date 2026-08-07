#pragma once
#include <iostream>

namespace honse {

    template<typename T, typename... Args>
    Resource<T> honse::ResourceManager::Load(const std::string& key, Args&&... args)
    {
        auto& resources = ResourceCache<T>::Resources;

        auto hashKey = hash(key); 
        auto it = resources.find(hashKey);

        if (it != ResourceCache<T>::Resources.end())
            return it->second;

        auto object = std::make_shared<T>(args...);
        ResourceCache<T>::Resources[hashKey] = object;

        std::cout << "Loading " << typeid(T).name() << " \'" << key << "' (" <<  hashKey << ")" << '\n';

        return object;
    }

}