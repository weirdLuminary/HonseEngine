#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include <honse/platform/Window.h>
#include <honse/graphics/Camera.h>

namespace honse {

    class Math {

    public:

        // Returns the angle in degrees needed to make the point at position 'from' face the point at position 'to'.
        static float rotateTowards(glm::vec2 from, glm::vec2 to) {
            return glm::degrees(atan2f(to.y - from.y, to.x - from.x));
        }

        static glm::vec2 toWorldPosition(glm::vec2 pos) {
            glm::vec2 view = honse::Camera::GetViewportSize();
            glm::vec2 screenSize = honse::Window::GetCurrentWindow()->GetSize();

            float x = (pos.x / screenSize.x) * view.x;
            float y = (1.0f - pos.y / screenSize.y) * view.y;

            return honse::Camera::GetMainCamera()->position + glm::vec2(x, y);
        }

        static glm::vec2 fromRelativePosition(glm::vec2 pos, glm::vec2 size) {
            return pos * size;
        }


    };
}