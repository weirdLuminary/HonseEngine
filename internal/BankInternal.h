#pragma once
#include <honse/audio/Bank.h>
#include <fmod_studio.hpp>

namespace honse {

    struct Bank::Impl {

        FMOD::Studio::Bank* bank;

    };
}