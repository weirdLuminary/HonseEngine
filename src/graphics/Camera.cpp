#include <honse/graphics/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

std::unique_ptr<hs::Camera> hs::Camera::m_Main;

hs::Camera* hs::Camera::GetMainCamera() {
    return m_Main.get();
}

glm::mat4 hs::Camera::getViewMatrix() {
    float angle = glm::radians(m_Main->rotation + 90.0f);
    return glm::lookAt(glm::vec3(m_Main->position, 0.0f), glm::vec3(m_Main->position, 1.0f), { cos(angle), sin(angle), 0.0f });
}

void hs::Camera::Init() {
    m_Main = std::make_unique<hs::Camera>();
}