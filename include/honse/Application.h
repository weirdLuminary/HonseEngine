#pragma once

namespace hs {

    class Application {

    public:
        Application();
        virtual ~Application();

        virtual void Run();

    };


    Application* CreateApplication();

};