#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <cstdint>

namespace hs {

    typedef std::uint64_t HashString;

    template<typename T>
    class ResourceCache
    {
        static std::unordered_map<HashString, std::shared_ptr<T>> Resources;

        friend class ResourceManager;
    };

    template<typename T>
    std::unordered_map<HashString, std::shared_ptr<T>> ResourceCache<T>::Resources;


    class ResourceManager {

    public:

        ResourceManager() = delete;

        template<typename T, typename... Args>
        static std::shared_ptr<T> Load(const std::string& path, Args&&... args);


    };

}
#include "ResourceManager.tpp"