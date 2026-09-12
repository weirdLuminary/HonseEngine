#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <honse/components/Transform.h>
#include <honse/resources/Resource.h>
#include <memory>

namespace honse {

class Shader;

namespace components {
struct Renderable;
struct AtlasRenderable;
} // namespace components

/**
 * @brief Main renderer class.
 *
 * Provides simple functions for rendering based on pre-defined components.
 *
 * @see honse::components
 */
class Renderer {

    public:
        Renderer() = delete;

        /**
         * @brief Initializes the renderer.
         *
         * @note NOT for manual use!
         */
        static void Init();

        /**
         * @brief Shuts down the renderer.
         *
         * @note NOT for manual use!
         */
        static void Shutdown();

        // clang-format off

        /**  
         * @brief Submits a sprite to the batch to render next.
         * 
         * @note Useful in case of manual rendering. Otherwise, use the built-in components with LECS.
         * 
         * @param renderable The AtlasRenderable struct with the texture and tint.
         * @param transform The Transform to modify vertex positions with.
         * 
         * @see honse::components::Renderable
         * @see honse::components::Transform
        */
        static void Submit(components::Renderable renderable,
            components::Transform transform);

        /**  
         * @brief Submits an atlas sprite to the batch to render next.
         * 
         * @note Useful in case of manual rendering. Otherwise, use the built-in components with LECS.
         * 
         * @param renderable The AtlasRenderable struct with the texture, tint & atlas frame.
         * @param transform The Transform to modify vertex positions with.
         * 
         * @see honse::components::AtlasRenderable
         * @see honse::components::Transform
        */
        static void Submit(components::AtlasRenderable renderable,
            components::Transform transform); // Submit atlas texture for drawing; useful in case of manual rendering, etc. Otherwise, use the AtlasRenderable component

        /**  
         * @brief Adds a post-processing shader to be applied to the end frame.
         * 
         * If none are added, post-processing is ignored and instead the frame will be output directly.
         * 
         * @param shader The Shader resource to apply post-processing with.
         * 
         * @see honse::Shader
         * @see honse::ResourceManager
        */
        static void AddPostProcessingShader(
            Resource<Shader> shader); // Add post-processing shader; modifies the resulting frame.

        // clang-format on

        static void OnResolutionChange(glm::vec2 resolution);

    private:
        static void Begin();
        static void End();
        static void Flush();

        friend class Engine;

        static const size_t MAX_SPRITES = 10000; // Max amount of sprites per frame
        static const int MAX_TEXTURES = 16;      // Max amount of loaded textures before Flush is called

        struct Impl;
        static std::unique_ptr<Impl> impl;
};

} // namespace honse