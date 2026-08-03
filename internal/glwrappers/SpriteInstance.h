#pragma once
#include <glm/glm.hpp>

struct SpriteInstance
{
    glm::vec4 tint;
    glm::vec2 position;
    float rotation;
    glm::vec2 scale;
    glm::vec2 pivot;
    glm::vec2 size;
    int textureSlot;
};