#pragma once
#include <memory>
#include <string>
#include <honse/modules/resources/ResourceManager.h>

namespace honse {

    class Bank;

    class Audio {

    public:

        Audio() = delete;

        static void Init();
        static void Shutdown();

        static void Update();

        //#ifdef FMOD_STUDIO

        static Resource<honse::Bank> LoadBank(const std::string& id, const std::string& path);

        static void PlayEvent(const std::string& id);

        //#endif

    private:

        struct Impl;
        static std::unique_ptr<Impl> impl;

        friend class Bank;

    };

};