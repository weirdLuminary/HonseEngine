#pragma once
#include <memory>

namespace honse {

    class Physics {

    public:

        Physics() = delete;

        // TBA

    private:

        struct Impl;
        std::unique_ptr<Impl> impl;

    };


}