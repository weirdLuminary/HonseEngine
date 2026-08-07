#pragma once
#include <memory>
#include <string>

namespace honse {

    class Audio {

    public:

        Audio() = delete;

        static void Init();
        static void Shutdown();

        static void Update();

        static void LoadBank(std::string& id, std::string& path);

    private:

        struct Impl;
        static std::unique_ptr<Impl> impl;

    };

};