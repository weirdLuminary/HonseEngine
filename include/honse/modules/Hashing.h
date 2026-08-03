#pragma once
#include <string_view>
#include <cstdint>

namespace honse {

    using HashString = uint64_t;

    // FNV1A hash
    constexpr uint64_t hash(std::string_view str) {
        uint64_t hash = 0xcbf29ce484222325ULL;
        for (char c : str) {
            hash ^= static_cast<uint64_t>(c);
            hash *= 0x00000100000001B3ULL;
        }
        return hash;
    }

    constexpr uint64_t operator""ID(const char* s) {
        return hash(s);
    }

}