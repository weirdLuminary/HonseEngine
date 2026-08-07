#pragma once
#include <cstdint>
#include <variant>
#include <unordered_map>
#include "ScopedTimer.h"

namespace honse {

    using ProfileValue = std::variant<int, int64_t, float, double>;
    

    class Profiling {

    public:

        Profiling() = delete;

        static void FlushData();
        static void Set(const char* name, ProfileValue value);

    private:

        static std::unordered_map<const char*, ProfileValue> m_Info;

    };

}