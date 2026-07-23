#pragma once
#include <memory>
#include <lecs/lecs.h>

namespace hs
{
    
    class Scene {

    public:

        Scene();

        World& GetWorld();

    private:

        World m_World;

    };
    
}
