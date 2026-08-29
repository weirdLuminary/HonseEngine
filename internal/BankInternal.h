#pragma once
#include <fmod_studio.hpp>
#include <honse/audio/Bank.h>

namespace honse {

struct Bank::Impl {

    FMOD::Studio::Bank* bank;
};
} // namespace honse