#include "glfwbackend/Input.hpp"
#include <honse/modules/Math.hpp>
#include <honse/platform/Input.h>
#include <iostream>

bool honse::Input::IsKeyDown(Key key) {

    return glfwGetKey(honse::Window::GetCurrentWindow()->m_Window,
                      toGLFW(key)) == GLFW_PRESS;
}

bool honse::Input::IsMouseButtonDown(int button) {

    return glfwGetMouseButton(honse::Window::GetCurrentWindow()->m_Window,
                              button) == GLFW_PRESS;
}

// Returns the position of the mouse cursor in world coordinates.
glm::vec2 honse::Input::GetMousePosition() {

    double x, y;

    glfwGetCursorPos(honse::Window::GetCurrentWindow()->m_Window, &x, &y);

    return honse::Math::toWorldPosition({x, y});
}