#include <honse/platform/Input.h>
#include <GLFW/glfw3.h>

bool Input::IsKeyDown(int key) {

    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;

} 

bool Input::IsMouseButtonDown(int button) {

    return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;

}