#pragma once
#include <glm/glm.hpp>

#pragma pack(push, 1)
struct SpriteVertex {
    glm::vec4 position;
    glm::vec2 uv;    
    glm::vec4 color;
    int textureID;
};
#pragma pack(pop)