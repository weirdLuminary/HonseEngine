#pragma once
#include <stdio.h>

#include "Application.h"
#include "Engine.h"

extern honse::Application* honse::CreateApplication();

// Platform-agnostic entry point wrapper
int main(int argc, char** argv) {

    printf("Engine bootstrapping...\n");

    auto* application = honse::CreateApplication();

    honse::Engine::Init(application);
    honse::Engine::Run();
    honse::Engine::Shutdown();

    return 0;
}
