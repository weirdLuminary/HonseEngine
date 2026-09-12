#pragma once
#include <lecs/lecs.h>
#include <memory>

namespace honse {

class Scene {

    public:
        Scene();

        lecs::World& GetWorld();

    private:
        lecs::World m_World;
};

} // namespace honse
