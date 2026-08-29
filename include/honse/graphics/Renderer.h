#pragma once
#include <honse/modules/resources/Resource.h>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"

namespace honse {

    extern float deltaTime;

    class Shader;

    class Renderer {

    public:
        
        Renderer() = delete;

        static void Init();
        static void Shutdown();
        
        static void Submit(Resource<Texture> texture, glm::vec2& position, float rotation, glm::vec2& scale, glm::vec4& tint, glm::vec2& pivot); // Submit texture for drawing; useful in case of manual culling, etc. Otherwise, use the Renderable component

        static void AddPostProcessingShader(Resource<Shader> shader); // Add post-processing shader; modifies the resulting frame.

        static void OnResolutionChange(glm::vec2 resolution);

    private:

        static void Begin();
        static void End();
        static void Flush();

        friend class Engine;

        struct Impl;
        static std::unique_ptr<Impl> impl;

    };

}