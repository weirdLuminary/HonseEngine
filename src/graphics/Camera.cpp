#include <glm/gtc/matrix_transform.hpp>
#include <honse/graphics/Camera.h>
#include <honse/graphics/Renderer.h>
#include <iostream>

std::unique_ptr<honse::Camera> honse::Camera::m_Main;

honse::Camera* honse::Camera::GetMainCamera() { return m_Main.get(); }

glm::vec2 honse::Camera::GetViewportSize() { return m_Main->m_ViewportSize; }

AABB honse::Camera::GetViewport() { return m_Main->m_Viewport; }

glm::mat4 honse::Camera::getViewMatrix() {
    glm::mat4 view(1.0f);
    view = glm::rotate(view, glm::radians(-m_Main->rotation), {0, 0, 1});
    view =
        glm::translate(view, {-m_Main->position.x, -m_Main->position.y, 0.0f});

    return view;
}

void honse::Camera::Init() { m_Main = std::make_unique<honse::Camera>(); }