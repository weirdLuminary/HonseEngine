#include <honse/audio/Audio.h>
#include <fmod/studio/fmod_studio.hpp>
#include <cstdio>
#include <iostream>
#include <BankInternal.h>

std::unique_ptr<honse::Audio::Impl> honse::Audio::impl;

struct honse::Audio::Impl {

    FMOD::Studio::System* system = nullptr;

    bool LoadFMODBank(honse::Bank& bank, const std::string& path)
    {
        FMOD::Studio::Bank* fmodBank = nullptr;

        FMOD_RESULT result = impl->system->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &fmodBank);

        if (result != FMOD_OK)
            return false;

        result = fmodBank->loadSampleData();

        if (result != FMOD_OK)
        {
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

    result = FMOD::Studio::System::create(&impl->system);
    if (result != FMOD_OK)
    {
        printf("FMOD error!\n");
    }

    result = impl->system->initialize(128, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        printf("FMOD error!\n");
    }
    printf("Initialized FMOD\n");
}

Resource<honse::Bank> honse::Audio::LoadBank(const std::string& id, const std::string& path) {

    Resource<honse::Bank> bank = honse::ResourceManager::Construct<honse::Bank>(id,
    [&]()
    {
        auto bank = honse::Bank::Create();

        if (!impl->LoadFMODBank(*bank, path))
            return Resource<honse::Bank>{};

        return bank;
    });

    return bank;
}

void honse::Audio::Update() {
    impl->system->update();
}

void honse::Audio::Shutdown() {
    impl->system->release();
}