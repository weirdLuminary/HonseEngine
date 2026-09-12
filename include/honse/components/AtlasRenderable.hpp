#pragma once
#include "Transform.h"
#include <honse/graphics/Camera.h>
#include <honse/graphics/Renderer.h>
#include <honse/graphics/Texture.h>
#include <honse/profiling/ScopedTimer.h>
#include <honse/resources/Resource.h>
#include <honse/utilities/AABB.h>
#include <honse/utilities/Time.h>
#include <lecs/lecs.h>

namespace honse::components {

struct AtlasFrame {
    public:
        glm::vec2 position;
        glm::vec2 size;
};

struct AtlasAnimation {
    public:
        std::vector<AtlasFrame> frames;
        float fps;
};

struct AnimatedAtlasRenderable {
    public:
        Resource<Texture> atlasTexture;
        Resource<AtlasAnimation> animation;
        size_t currentFrame = 0;
        float animationTimer = 0.0f;
        glm::vec4 tint = glm::vec4(1.0f);
};

struct AtlasRenderable {
    public:
        Resource<Texture> atlasTexture;
        AtlasFrame frame;
        glm::vec4 tint = glm::vec4(1.0f);
};

class AtlasRenderableHandler : public lecs::System {
    public:
        void Init(lecs::World& world) override {}

        void Update(lecs::World& world) override {

            auto view = world.GetView<AtlasRenderable, Transform>();

            for (auto [ent, renderable, transform] : view) {

                AABB box = getRotatedAABB(transform.position,
                    renderable.frame.size * transform.scale,
                    transform.origin,
                    transform.rotationRadians); // Get basic AABB for culling

                if (isColliding(box,
                        honse::Camera::GetViewport())) { // Check culling collision
                    honse::Renderer::Submit(renderable, transform);
                }
            }

            auto animatedView = world.GetView<AnimatedAtlasRenderable, Transform>();

            for (auto [ent, renderable, transform] : animatedView) {

                AtlasFrame& currentFrame = renderable.animation->frames[renderable.currentFrame];

                AABB box = getRotatedAABB(transform.position,
                    currentFrame.size * transform.scale,
                    transform.origin,
                    transform.rotationRadians); // Get basic AABB for culling

                if (isColliding(box,
                        honse::Camera::GetViewport())) { // Check culling collision
                    honse::Renderer::Submit(
                        { renderable.atlasTexture, currentFrame, renderable.tint }, transform);
                }

                renderable.animationTimer += honse::Time::GetDeltaTime();

                const float frameDelay =
                    1.0f / renderable.animation->fps; // Delay per one frame of the animation

                const int frameCount = static_cast<int>(
                    renderable.animationTimer / frameDelay); // Count of frames to progress the animation

                renderable.animationTimer -= frameCount * frameDelay;

                renderable.currentFrame =
                    (renderable.currentFrame + frameCount) % renderable.animation->frames.size();
            }
        }
};

} // namespace honse::components