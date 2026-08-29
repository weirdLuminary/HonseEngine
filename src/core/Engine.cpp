#include <honse/Application.h>
#include <honse/Engine.h>
#include <honse/audio/Audio.h>
#include <honse/graphics/Camera.h>
#include <honse/graphics/Renderer.h>
#include <honse/modules/Threading.h>
#include <honse/modules/Time.h>
#include <honse/modules/profiling/Profiling.h>
#include <honse/modules/resources/ResourceManager.h>
#include <honse/objects/SceneManager.h>

honse::Application* honse::Engine::m_Application;
honse::Window* honse::Engine::m_Window;

void honse::Engine::CreateWindow() {
    m_Window = new honse::Window(640, 640, "test");
    if (m_Window == nullptr)
        Shutdown(-1);

    honse::Window::BindWindow(m_Window);
}

void honse::Engine::Init(Application* app) {
    m_Application = app;

#ifdef DEBUG
#pragma message("!!! Debugging enabled !!!")
#endif

    // Primary systems
    honse::Window::Initialize();
    CreateWindow();
    honse::Threading::Init();
    honse::Audio::Init();
    honse::Camera::Init();
    honse::Renderer::Init();

    // Secondary systems
    honse::Time::Init();

    m_Application->Main();
}

void honse::Engine::Run() {
    while (!m_Window->ShouldClose()) {

        honse::Time::StartFrame();

        m_Window->PollEvents();

        honse::SceneManager::StartFrame();

        m_Application->OnUpdate();

        honse::Renderer::Begin();
        honse::SceneManager::Update();
        honse::Renderer::End();

        honse::SceneManager::EndFrame();

        m_Window->SwapBuffers();

#ifdef DEBUG
        honse::Profiling::FlushData();
#endif

        honse::Audio::Update();

        honse::Time::EndFrame();
    }
}

void honse::Engine::Shutdown(int code) {

    honse::Threading::Shutdown();
    honse::SceneManager::Shutdown();
    honse::ResourceManager::Shutdown();
    honse::Renderer::Shutdown();
    honse::Audio::Shutdown();

    delete m_Window;
}
