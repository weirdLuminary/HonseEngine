#pragma once
#include "Transform.h"
#include <honse/graphics/Camera.h>
#include <honse/graphics/Renderer.h>
#include <honse/graphics/Texture.h>
#include <honse/profiling/ScopedTimer.h>
#include <honse/resources/Resource.h>
#include <honse/utilities/AABB.h>
#include <lecs/lecs.h>

namespace honse::components {

struct Renderable {
        Resource<Texture> texture;
        glm::vec4 tint = glm::vec4(1.0f);
};

class RenderableHandler : public lecs::System {
    public:
        void Init(lecs::World& world) override {}

        void Update(lecs::World& world) override {

            auto view = world.GetView<Renderable, Transform>();

            for (auto [ent, renderable, transform] : view) {

                glm::vec2& texSize = renderable.texture->size;

                AABB box = getRotatedAABB(transform.position,
                    texSize * transform.scale,
                    transform.origin,
                    transform.rotationRadians);

                if (isColliding(box, honse::Camera::GetViewport())) {
                    honse::Renderer::Submit(renderable, transform);
                }
            }
        }
};

} // namespace honse::components