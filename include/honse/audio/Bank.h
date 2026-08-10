#pragma once
#include <memory>

namespace honse {

    class Bank {

    public:

        static std::shared_ptr<Bank> Create();

        ~Bank();

    private:

        Bank();

        struct Impl;
        std::unique_ptr<Impl> impl;

        friend class Audio;

    };


}