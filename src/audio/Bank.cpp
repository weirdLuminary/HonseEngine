#include <BankInternal.h>
#include <fmod/studio/fmod_studio.hpp>
#include <honse/audio/Audio.h>

honse::Bank::Bank() : impl(std::make_unique<Impl>()) {}

honse::Bank::~Bank() { impl->bank->unload(); }

std::shared_ptr<honse::Bank> honse::Bank::Create() {
    return std::shared_ptr<honse::Bank>(new honse::Bank());
}