#pragma once
#include "platform/Window.h"

namespace honse {

class Application {

  public:
    Application();
    virtual ~Application();

    virtual void Main();     // Application entry point that runs once.
    virtual void OnUpdate(); // Function that runs every frame independently.
                             // Useful for interacting with rendering context.
};

Application* CreateApplication();

}; // namespace honse