#pragma once
#include <glm/glm.hpp>
#include "Sprite.h"
#include <memory>
namespace hs {

    class Renderer {

    public:
        
        Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Begin();
        static void End();

        static void Submit(const hs::Sprite& sprite);
        static void Flush();

    private:

        struct Impl;
        static std::unique_ptr<Impl> impl;


    };

}