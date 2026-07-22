#pragma once
#include <glm/glm.hpp>

namespace hs {

    struct TransformComponent {

        glm::vec2 position = glm::vec2(0.0f);
        glm::vec2 pivot = glm::vec2(0.5f);
        float rotation = 0.0f;

    };

}