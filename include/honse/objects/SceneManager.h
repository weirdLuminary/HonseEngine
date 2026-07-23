#pragma once
#include "Scene.h"

namespace hs
{

    class SceneManager {

    public:

        SceneManager() = delete;

        static const std::vector<std::unique_ptr<Scene>>& GetScenes(); 

        static Scene& Load(std::unique_ptr<Scene> scene);
        static void Unload(Scene& scene);

        static void Update();

    private:

        static std::vector<std::unique_ptr<Scene>> m_ActiveScenes;

        friend Scene;

    };
    
}
