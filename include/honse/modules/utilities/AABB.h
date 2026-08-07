#pragma once
#include <cmath>
#include <honse/modules/profiling/ScopedTimer.h>
#include <iostream>

struct AABB
{
    float left, top, right, bottom;
};

inline AABB getRotatedAABB(glm::vec2 position, glm::vec2 size, glm::vec2 pivot, float angle)
{

    glm::vec2 invPivot = (1.0f - pivot);
    glm::vec2 corners[4] =
    {
        {-pivot.x * size.x,     -pivot.y * size.y   }, // Left bottom
        { invPivot.x * size.x,  -pivot.y * size.y   }, // Right top
        { invPivot.x * size.x,  invPivot.y * size.y },
        {-pivot.x * size.x,     invPivot.y * size.y }
    };


    AABB box {
        FLT_MAX,
        FLT_MAX,
        -FLT_MAX,
        -FLT_MAX
    }; // NOTE: Value initialization is needed for min & max later in the function

    float c = cos(angle);
    float s = sin(angle);

    for (auto& corner : corners)
    {
        glm::vec2 rotated =
        {
            corner.x * c - corner.y * s,
            corner.x * s + corner.y * c
        };

        glm::vec2 world = position + rotated;

        box.left   = std::min(box.left, world.x);
        box.right  = std::max(box.right, world.x);
        box.top    = std::min(box.top, world.y);
        box.bottom = std::max(box.bottom, world.y);
    }

    return box;

}

inline bool isColliding(AABB a, AABB b) {
    return  a.left   < b.right  &&
            a.right  > b.left   &&
            a.top    < b.bottom &&
            a.bottom > b.top;
}