#pragma once
#include "Object.h"
#include <memory>

namespace hs
{
    
    class Scene {

    public:

        Scene() = default;

        void Add(std::unique_ptr<Object> obj);
        
        const std::vector<std::unique_ptr<Object>>& GetEntities() const;

        template<typename T, typename... Args>
        T& Create(Args&&... args)
        {
            static_assert(std::is_base_of_v<Object, T>);

            auto object = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *object;

            m_Entities.push_back(std::move(object));

            return ref;
        }

    private:

        std::vector<std::unique_ptr<Object>> m_Entities;

    };
    
}
