#pragma once
#include "platform/Window.h"

namespace hs {

    class Application {

    public:
        Application();
        virtual ~Application();

        virtual void Start();
        virtual void Update();

    };


    Application* CreateApplication();

};