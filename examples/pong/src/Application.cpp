#include <honse/Engine.h>

class Pong : public hs::Application {

public:

    Pong() {
        
    }

    ~Pong() {


    }

    void Run() {
        while (true)
        {
            /* code */
        }
        
    }

};

hs::Application* hs::CreateApplication() {
    return new Pong();
}