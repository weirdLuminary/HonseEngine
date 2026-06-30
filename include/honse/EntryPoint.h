#pragma once

#include "Application.h"
#include <iostream>

extern hs::Application* hs::CreateApplication();

// Platform-agnostic entry point wrapper
int main(int argc, char** argv) {
    std::cout << "Engine Bootstrapping..." << std::endl;
    
    auto app = hs::CreateApplication();
    
    app->Run();
    
    delete app;
    return 0;
}
