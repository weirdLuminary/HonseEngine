#pragma once

namespace honse {

class Application;
class Window;

class Engine {

  public:
    Engine() = delete;

    static void Init(Application* application);
    static void Run();
    static void Shutdown(int code = 0);

  private:
    static void CreateWindow();

    static Application* m_Application;
    static Window* m_Window;
};

} // namespace honse