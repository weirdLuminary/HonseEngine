#include <honse/Engine.h>

class Pong : public honse::Application {

public:

    Pong();
    ~Pong();

    void Start();

};

honse::Application* honse::CreateApplication() {
    return new Pong();
}