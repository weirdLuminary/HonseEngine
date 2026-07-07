#pragma once
#include <honse/objects/TransformObject.h>
#include <honse/graphics/Material.h>

namespace hs {

    class Sprite : public hs::TransformObject {

    public:

        Sprite(Texture* texture);
        Sprite(Material* material);
        
        glm::vec2 GetSize() const;

        glm::vec4 color = glm::vec4(1.0f);

        Material* material;

    private:

        glm::vec2 m_Size;

    };

}