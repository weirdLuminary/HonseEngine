#pragma once
#include "Resource.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <cstdint>
#include <honse/modules/Hashing.h>

namespace honse {

    template<typename T>
    class ResourceCache
    {
        static std::unordered_map<HashString, Resource<T>> Resources;

        friend class ResourceManager;
    };

    template<typename T>
    std::unordered_map<HashString, Resource<T>> ResourceCache<T>::Resources;


    class ResourceManager {

    public:

        ResourceManager() = delete;

        template<typename T, typename... Args>
        static Resource<T> Load(const std::string& path, Args&&... args);


    };

}
#include "ResourceManager.tpp"