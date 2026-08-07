#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <honse/modules/utilities/AABB.h>

namespace honse {

    class Camera {

    public:

        glm::vec2 position = glm::vec2(0.0f);
        float rotation = 0.0f;

        Camera() = default;

        static glm::vec2 GetViewportSize();
        static AABB GetViewport();

        static Camera* GetMainCamera();
        
        static void Init();

    private:

        static glm::mat4 getViewMatrix();
        static std::unique_ptr<Camera> m_Main;

        glm::vec2 m_ViewportSize;
        AABB m_Viewport;

        friend class Renderer;

    };

}