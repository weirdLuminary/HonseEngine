#include <honse/audio/Audio.h>
#include <fmod/studio/fmod_studio.hpp>
#include <cstdio>

std::unique_ptr<honse::Audio::Impl> honse::Audio::impl;

struct honse::Audio::Impl {

    FMOD::Studio::System* system = nullptr;

};

void honse::Audio::Init() {

    impl = std::make_unique<Impl>();

    FMOD_RESULT result;

    result = FMOD::Studio::System::create(&impl->system);
    if (result != FMOD_OK)
    {
        printf("FMOD error!\n");
    }

    result = impl->system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        printf("FMOD error!\n");
    }
    printf("FMOD success!\n");
}

void honse::Audio::Update() {
    impl->system->update();
}

void honse::Audio::Shutdown() {
    impl->system->release();
}