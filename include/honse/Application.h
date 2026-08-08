#pragma once
#include "platform/Window.h"

namespace honse {

    class Application {

    public:
        Application();
        virtual ~Application();

        virtual void Main();

    };


    Application* CreateApplication();

};