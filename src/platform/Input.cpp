#include <honse/platform/Input.h>
#include <GLFW/glfw3.h>

bool hs::Input::IsKeyDown(Key key) {

    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;

} 

bool hs::Input::IsMouseButtonDown(int button) {

    return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;

}

glm::vec2 hs::Input::GetMousePosition() {

    double x, y;
    glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
    return { x, y };

}