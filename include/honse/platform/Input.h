#pragma once
#include <glm/glm.hpp>

class Input
{
public:
    static bool IsKeyDown(int key);
    static bool IsMouseButtonDown(int button);

    static glm::vec2 GetMousePosition();

};
