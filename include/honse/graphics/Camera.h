#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <honse/objects/TransformObject.h>

namespace hs {

    class Camera : public hs::TransformObject {

    public:

        Camera() = default;

        static Camera* GetMainCamera();
        
        static void Init();

    private:

        static glm::mat4 getViewMatrix();
        static std::unique_ptr<Camera> m_Main;

        friend class Renderer;

    };

}