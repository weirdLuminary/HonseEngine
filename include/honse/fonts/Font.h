#pragma once
#include <memory>

namespace honse {

class Font {

    public:
    private:
        struct Impl;
        static std::unique_ptr<Impl> impl;
};

} // namespace honse