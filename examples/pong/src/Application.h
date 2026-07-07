#include <honse/Engine.h>

class Pong : public hs::Application {

public:

    Pong();
    ~Pong();

    void Start();
    void Update();

};

hs::Application* hs::CreateApplication() {
    return new Pong();
}