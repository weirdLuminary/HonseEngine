#pragma once
#include <honse/modules/resources/Resource.h>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"

namespace honse {

    class Renderer {

    public:
        
        Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Begin();
        static void End();

        static void Submit(Resource<Texture> texture, glm::vec2& position, float rotation, glm::vec2& scale, glm::vec4& tint, glm::vec2& pivot);
        static void Flush();

        static void OnResolutionChange(glm::vec2 resolution);

    private:

        struct Impl;
        static std::unique_ptr<Impl> impl;

    };

}