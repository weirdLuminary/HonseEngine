#pragma once
#include "Resource.h"
#include <honse/modules/Hashing.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <cstdint>
#include <vector>

namespace honse {

    class IResourceCache
    {
    public:
        virtual ~IResourceCache() = default;
        virtual void Clear() = 0;
    };

    template<typename T>
    class ResourceCache : public IResourceCache
    {
    public:
        template<typename... Args>
        Resource<T> Load(HashString hashKey, Args... args)
        {
            auto it = m_Resources.find(hashKey);

            if (it != m_Resources.end())
                return it->second;

            auto resource = std::make_shared<T>(
                std::forward<Args>(args)...
            );

            m_Resources.emplace(hashKey, resource);

            printf("Type %s loaded with ID %i\n", typeid(T).name(), hashKey);

            return resource;
        }

        template<typename Factory>
        Resource<T> Construct(HashString hashKey, Factory&& factory)
        {
            auto it = m_Resources.find(hashKey);

            if (it != m_Resources.end())
                return it->second;

            auto resource = factory();

            m_Resources.emplace(hashKey, resource);

            printf("Type %s loaded via construction with ID %i\n", typeid(T).name(), hashKey);

            return resource;
        }

        void Clear() override
        {
            m_Resources.clear();
        }

    private:
        std::unordered_map<HashString, Resource<T>> m_Resources;
    };


    class ResourceManager {

    public:

        ResourceManager() = delete;

        static void Shutdown();

        template<typename T, typename... Args>
        static Resource<T> Load(const std::string& key, Args&&... args);

        template<typename T, typename Factory>
        static Resource<T> Construct(const std::string& key, Factory&& factory);

    private:

        template<typename T>
        static ResourceCache<T>& GetCache();
        
        static std::vector<IResourceCache*> m_Caches;

    };

}
#include "ResourceManager.tpp"