#pragma once
#include <glm/glm.hpp>

struct SpriteVertex {
    glm::vec4 position;
    glm::vec2 uv;    
    glm::vec4 color;
    int textureID;
};