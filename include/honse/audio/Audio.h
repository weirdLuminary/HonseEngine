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

        static Resource<honse::Bank> LoadBank(const std::string& id, const std::string& path);

    private:

        struct Impl;
        static std::unique_ptr<Impl> impl;

        friend class Bank;

    };

};