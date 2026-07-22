#pragma once
#include <chrono>
#include <stdio.h>

namespace hs {

    class ScopedTimer
    {
    public:
        ScopedTimer(const char* name)
            : m_Name(name)
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        ~ScopedTimer();

    private:

        const char* m_Name;
        std::chrono::high_resolution_clock::time_point m_Start;
    };

}