#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace hs {

    class Camera {

    public:

        glm::vec2 position = glm::vec2(0.0f);
        float rotation = 0.0f;

        Camera() = default;

        static Camera* GetMainCamera();
        
        static void Init();

    private:

        static glm::mat4 getViewMatrix();
        static std::unique_ptr<Camera> m_Main;

        friend class Renderer;

    };

}