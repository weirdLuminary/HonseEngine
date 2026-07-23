#pragma once
#include <glm/glm.hpp>

namespace hs {

    struct Transform {

        glm::vec2 position = glm::vec2(0.0f);
        float rotation = 0.0f;
        glm::vec2 scale = glm::vec2(1.0f);
        glm::vec2 pivot = glm::vec2(0.5f);

    };

}