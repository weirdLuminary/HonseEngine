#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

namespace honse
{

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    static void Initialize();

    bool ShouldClose() const;
    void SwapBuffers() const;
    void PollEvents() const;

    glm::vec2 GetSize() const;

    static void BindWindow(Window* window);
    static const Window* GetCurrentWindow();

    //GLFWwindow* NativeHandle() const;

private:
    GLFWwindow* m_Window = nullptr;

    static Window* m_CurrentWindow;

    friend class Input;
};

}