#pragma once
#include <GLFW/glfw3.h>
#include "Keymap.h"
#include <honse/platform/Input.h>


constexpr int toGLFW(honse::Input::Key key)
{
    switch (key)
    {
        #define X(hid, glfw) case honse::Input::Key::hid: return glfw;

        GLFW_KEY_MAP(X)

        #undef X

    default:
        return GLFW_KEY_UNKNOWN;
    }
}