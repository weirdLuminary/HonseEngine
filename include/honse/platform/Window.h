#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

namespace hs
{

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    bool ShouldClose() const;
    void Update() const;

    glm::vec2 GetSize();

    GLFWwindow* NativeHandle() const;

private:
    GLFWwindow* m_Window = nullptr;
};

}