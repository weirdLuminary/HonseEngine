#pragma once
#include <stdint.h>
#include <string.h>

uint64_t lum_hash(const char* str) {
    uint64_t hash = 0xcbf29ce484222325ULL;
    size_t length = strlen(str);
    for (int i = 0; i < length; i++) {
        char c = str[i];
        hash ^= (uint64_t)c;
        hash *= 0x00000100000001B3ULL;
    }
    return hash;
}