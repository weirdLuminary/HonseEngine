#pragma once
#include "Preferences.h"
#include <stdint.h>

namespace honse {

class HonseProject {

    public:
        HonseProject();
        virtual ~HonseProject();

        virtual void Main();     // Application entry point that runs once.
        virtual void OnUpdate(); // Function that runs every frame independently. Useful for interacting with
                                 // rendering context.

        void UpdatePreferences();

        ProjectPreferences preferences;
};

HonseProject* CreateProject();

}; // namespace honse