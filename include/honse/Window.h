#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace hs
{

class Window
{
public:
    Window(int width, int height, std::string_view title);
    ~Window();

    bool ShouldClose() const;

    void PollEvents();
    void SwapBuffers();

    int Width() const;
    int Height() const;

    GLFWwindow* NativeHandle() const;

private:
    GLFWwindow* m_Window = nullptr;

    int m_Width;
    int m_Height;
};

}