#include <BankInternal.h>
#include <cassert>
#include <cstdio>
#include <fmod/core/fmod_errors.h>
#include <fmod/studio/fmod_studio.hpp>
#include <honse/audio/Audio.h>
#include <iostream>

std::unique_ptr<honse::Audio::Impl> honse::Audio::impl;

struct honse::Audio::Impl {

    FMOD::Studio::System* system = nullptr;

    bool LoadFMODBank(honse::Bank& bank, const std::string& path) {
        FMOD::Studio::Bank* fmodBank = nullptr;

        FMOD_RESULT result = impl->system->loadBankFile(
            path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &fmodBank);

        std::cout << FMOD_ErrorString(result) << '\n';

        if (result != FMOD_OK)
            return false;

        result = fmodBank->loadSampleData();

        std::cout << FMOD_ErrorString(result) << '\n';

        if (result != FMOD_OK) {
            fmodBank->unload();
            return false;
        }

        bank.impl->bank = fmodBank;

        return true;
    }
};

void honse::Audio::Init() {

    impl = std::make_unique<Impl>();

    FMOD_RESULT result;

    FMOD::System* core = nullptr;

    result = FMOD::Studio::System::create(&impl->system);
    assert(result == FMOD_OK);

    result = impl->system->getCoreSystem(&core);
    assert(result == FMOD_OK);

    result = impl->system->initialize(128, FMOD_STUDIO_INIT_NORMAL,
                                      FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        printf("FMOD error!\n");
    }
    printf("Initialized FMOD\n");
}

void honse::Audio::PlayEvent(const std::string& id) {
    FMOD::Studio::EventDescription* description = nullptr;
    FMOD_RESULT result = impl->system->getEvent(id.c_str(), &description);

    assert(result == FMOD_OK);
    assert(description != nullptr);

    FMOD::Studio::EventInstance* event = nullptr;
    result = description->createInstance(&event);
    assert(result == FMOD_OK);

    result = event->start();
    assert(result == FMOD_OK);

    result = event->release();
}

Resource<honse::Bank> honse::Audio::LoadBank(const std::string& id,
                                             const std::string& path) {

    Resource<honse::Bank> bank =
        honse::ResourceManager::Construct<honse::Bank>(id, [&]() {
            auto bank = honse::Bank::Create();

            if (!impl->LoadFMODBank(*bank, path))
                return Resource<honse::Bank>{};

            return bank;
        });

    return bank;
}

void honse::Audio::Update() {
    FMOD_RESULT result = impl->system->update();
    assert(result == FMOD_OK);
}

void honse::Audio::Shutdown() { impl->system->release(); }