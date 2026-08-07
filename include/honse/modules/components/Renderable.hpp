#pragma once
#include <honse/graphics/Renderer.h>
#include <honse/graphics/Texture.h>
#include <honse/modules/resources/Resource.h>
#include "Transform.h"
#include <lecs/lecs.h>
#include <honse/modules/utilities/AABB.h>
#include <honse/graphics/Camera.h>
#include <honse/modules/profiling/ScopedTimer.h>

namespace honse {

    struct Renderable
    {
        Resource<Texture> texture;
        glm::vec4 tint = glm::vec4(1.0f);
    };
    
    class RenderableHandler : public System {
    public:
        void Init(World& world) override {}

        void Update(World& world) override {
            
            auto view = world.GetView<Renderable, Transform>();

            for(auto [ent, renderable, transform] : view) {

                // std::cout << "Entity: " << ent << "\n";

                glm::vec2& texSize = renderable.texture->size;
                float rotationRad = glm::radians(transform.rotation);

                AABB box = getRotatedAABB(transform.position, texSize, transform.pivot, rotationRad);
                
                if(isColliding(box, honse::Camera::GetViewport())) { 
                    honse::Renderer::Submit(renderable.texture, transform.position, rotationRad, transform.scale, renderable.tint, transform.pivot);
                }
            }
        }
    };

}