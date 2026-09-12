#pragma once

namespace honse {

class HonseProject;
class Window;

class Engine {

    public:
        Engine() = delete;

        static void Init(HonseProject* application);
        static void Run();
        static void Shutdown(int code = 0);

    private:
        static void CreateWindow();

        static HonseProject* m_Application;
        static Window* m_Window;
};

} // namespace honse