#pragma once
#include "Scene.h"

namespace honse
{

    class SceneManager {

    public:

        SceneManager() = delete;

        static const std::vector<std::unique_ptr<Scene>>& GetScenes(); 

        static Scene& Load(std::unique_ptr<Scene> scene);
        static Scene& Load(std::string& path);

        static Scene& CreateScene();

        static void Unload(Scene& scene);

        static void Update();

    private:

        static std::vector<std::unique_ptr<Scene>> m_ActiveScenes;

        friend Scene;

    };
    
}
