#include <honse/platform/Window.h>
#include <iostream>

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

hs::Window::Window(int w, int h, const char* title) {

    if (!glfwInit())
    {
        std::cout << "glfwInit failed\n";
        return;
    }
    std::cout << "glfwInit succeeded\n";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!m_Window)
    {
        printf("Window init fail!");
        return;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("GLAD init fail!");
        return;
    }    
    std::cout << "Window created\n";
}

void hs::Window::Update() const {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool hs::Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }

GLFWwindow* hs::Window::NativeHandle() const { return m_Window; }

glm::vec2 hs::Window::GetSize() { 
    int* width;
    int* height;

    glfwGetWindowSize(m_Window, width, height);

    return glm::vec2(*width, *height);
};

hs::Window::~Window() {

    glfwDestroyWindow(m_Window);
    glfwTerminate();

}