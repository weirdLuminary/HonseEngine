#include <glad/glad.h>
#include <honse/graphics/Renderer.h>
#include <honse/platform/Window.h>
#include <iostream>

honse::Window* honse::Window::m_CurrentWindow = nullptr;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    honse::Renderer::OnResolutionChange({width, height});
}

void honse::Window::BindWindow(Window* window) { m_CurrentWindow = window; }

const honse::Window* honse::Window::GetCurrentWindow() {
    return m_CurrentWindow;
}

honse::Window::Window(int w, int h, const char* title) {

    m_Window = glfwCreateWindow(w, h, title, NULL, NULL);
    if (!m_Window) {
        printf("Window initialization fail!");
        return;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("GLAD initialization fail!");
        return;
    }
}

void honse::Window::Initialize() {
    if (!glfwInit()) {
        std::cout << "GLFW initialization failure!\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(1); // Enable VSync
    glfwWindowHint(GLFW_SAMPLES, 4);
}

void honse::Window::PollEvents() const { glfwPollEvents(); }

void honse::Window::SwapBuffers() const { glfwSwapBuffers(m_Window); }

bool honse::Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

glm::vec2 honse::Window::GetSize() const {
    int width;
    int height;

    glfwGetFramebufferSize(m_Window, &width, &height);

    return glm::vec2(width, height);
};

honse::Window::~Window() {

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}