#pragma once

#include "Application.h"
#include "objects/SceneManager.h"
#include <iostream>

extern hs::Application* hs::CreateApplication();

// Platform-agnostic entry point wrapper
int main(int argc, char** argv) {
    std::cout << "Engine Bootstrapping..." << std::endl;

    #ifdef DEBUG
        std::cout << "!!! Debugging enabled !!!" << std::endl;
    #endif
    
    auto app = hs::CreateApplication();

    hs::Window* window = new hs::Window(640, 640, "test");   
    hs::Renderer::Init(); 
    
    app->Start();

    if(window == nullptr) return -1;

    while(!window->ShouldClose()) {
        app->Update();

        hs::Renderer::Begin();

        hs::SceneManager::Render();
        hs::Renderer::Flush();

        hs::Renderer::End();
        
        window->Update();
    }

    hs::Renderer::Shutdown(); 
    
    delete app;
    return 0;
}
