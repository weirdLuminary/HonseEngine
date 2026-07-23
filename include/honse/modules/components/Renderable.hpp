#pragma once
#include <honse/graphics/Renderer.h>
#include <honse/graphics/Texture.h>
#include <honse/modules/resources/Resource.h>
#include "Transform.h"
#include <lecs/lecs.h>

namespace hs {

    struct Renderable
    {
        Resource<Texture> texture;
        glm::vec4 tint = glm::vec4(1.0f);
    };
    
    class RenderableHandler : public System {
    public:
        void Update(Entity e, World& world) override {
            auto& transform = world.GetComponent<Transform>(e);
            auto& renderable = world.GetComponent<Renderable>(e);
            hs::Renderer::Submit(renderable.texture, transform.position, transform.rotation, transform.scale, renderable.tint, transform.pivot);
        }
    };

}