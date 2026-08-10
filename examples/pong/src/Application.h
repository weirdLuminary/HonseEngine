#include <honse/honse.h>

class Pong : public honse::Application {

public:

    Pong();
    ~Pong();

    void Main();

};

honse::Application* honse::CreateApplication() {
    return new Pong();
}