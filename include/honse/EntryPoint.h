#pragma once

#include "Application.h"
#include "objects/SceneManager.h"
#include <lecs/lecs.h>
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "modules/profiling/Profiling.h"
#include "platform/Input.h"
#include "audio/Audio.h"
#include <iostream>
#include <honse/modules/Threading.h>

extern honse::Application* honse::CreateApplication();

// Platform-agnostic entry point wrapper
int main(int argc, char** argv) {
    std::cout << "Engine Bootstrapping..." << std::endl;

    #ifdef DEBUG
    #pragma message("!!! Debugging enabled !!!")
    #endif

    #ifdef NDEBUG
    #pragma message("Release version")
    #endif

    honse::Threading::Init();
    
    auto app = honse::CreateApplication();

    honse::Window* window = new honse::Window(640, 640, "test"); 
    honse::Window::BindWindow(window);

    honse::Audio::Init();
    honse::Camera::Init();  
    honse::Renderer::Init(); 
    
    app->Start();

    if(window == nullptr) return -1;

    while(!window->ShouldClose()) {

        honse::Renderer::Begin();

        honse::SceneManager::Update();

        honse::Renderer::End();

        window->Update();

        #ifdef DEBUG
        honse::Profiling::FlushData();
        #endif
    }

    honse::Renderer::Shutdown(); 
    honse::Audio::Shutdown();
    honse::Threading::Shutdown();
    
    delete app;
    return 0;
}
