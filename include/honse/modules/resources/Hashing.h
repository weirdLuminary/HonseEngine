#pragma once
#include <string_view>
#include <cstdint>

namespace hs {

    // FNV1A hash
    constexpr std::uint64_t hash(std::string_view str) {
        std::uint64_t hash = 0xcbf29ce484222325ULL;
        for (char c : str) {
            hash ^= static_cast<std::uint64_t>(c);
            hash *= 0x00000100000001B3ULL;
        }
        return hash;
    }

    constexpr std::uint64_t operator""_hash(const char* s) {
        return hash(s);
    }

}