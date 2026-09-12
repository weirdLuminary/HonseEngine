#pragma once
#include <glm/glm.hpp>

namespace honse::components {

struct Transform {
    public:
        glm::vec2 position = glm::vec2(0.0f);
        float rotationRadians = 0.0f;
        glm::vec2 scale = glm::vec2(1.0f);
        glm::vec2 origin = glm::vec2(0.5f);
};

} // namespace honse::components