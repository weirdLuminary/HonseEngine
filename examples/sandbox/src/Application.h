#include <honse/honse.h>

class Sandbox : public honse::HonseProject {

public:

    Sandbox() {};
    ~Sandbox();

    void Main();
    void OnUpdate() {};

};

honse::HonseProject* honse::CreateProject() {
    return new Sandbox();
}